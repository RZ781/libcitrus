/* Copyright (C) 2025 RZ781
 *
 * This file is part of libcitrus.
 *
 * libcitrus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcitrus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "citrus.h"

typedef enum {
	CITRUS_EVENT_CONNECT,
	CITRUS_EVENT_DISCONNECT
} CitrusEventType;

typedef struct {
	CitrusEventType type;
	int client_id;
	int data;
} CitrusEvent;

void CitrusParser_init(CitrusParser* parser) {
	parser->read_pointer = 0;
	parser->write_pointer = 0;
}

bool CitrusParser_send(CitrusParser* parser, int n, uint8_t* data) {
	for (int i = 0; i < n; i++) {
		parser->buffer[parser->write_pointer++] = data[i];
		parser->write_pointer %= CITRUS_PARSER_BUFFER_SIZE;
		if (parser->write_pointer == parser->read_pointer) {
			return false;
		}
	}
	return true;
}

bool CitrusParser_get_event(CitrusParser* parser, CitrusEvent* event) {
	int length = parser->write_pointer - parser->read_pointer;
	if (length < 0) {
		length += CITRUS_PARSER_BUFFER_SIZE;
	}
	if (length < 4) {
		return false;
	}
	event->type = parser->buffer[parser->read_pointer++];
	parser->read_pointer %= CITRUS_PARSER_BUFFER_SIZE;
	event->client_id = parser->buffer[parser->read_pointer++];
	parser->read_pointer %= CITRUS_PARSER_BUFFER_SIZE;
	event->data = parser->buffer[parser->read_pointer++] << 8;
	parser->read_pointer %= CITRUS_PARSER_BUFFER_SIZE;
	event->data |= parser->buffer[parser->read_pointer++];
	parser->read_pointer %= CITRUS_PARSER_BUFFER_SIZE;
	return true;
}

void CitrusLobby_init(CitrusLobby* lobby) {
	for (int i = 0; i < 256; i++) {
		lobby->in_game_clients[i] = false;
		lobby->connected_clients[i] = false;
	}
}

void CitrusLobby_event(CitrusLobby* lobby, CitrusEvent event) {
	switch (event.type) {
		default:
			break;
	}
}

void CitrusClientLobby_init(CitrusClientLobby* lobby, void (*send)(void* send_data, int n, uint8_t* data), void* send_data) {
	CitrusParser_init(&lobby->parser);
	CitrusLobby_init(&lobby->lobby);
	lobby->send = send;
	lobby->send_data = send_data;
}

void CitrusClientLobby_recv(CitrusClientLobby* lobby, int n, uint8_t* data) {
	CitrusParser_send(&lobby->parser, n, data);
	CitrusEvent event;
	while (CitrusParser_get_event(&lobby->parser, &event)) {
		CitrusLobby_event(&lobby->lobby, event);
	}
}

void CitrusServerLobby_init(CitrusServerLobby* lobby, void (*send)(void* send_data, int n, uint8_t* data, int id), void* send_data) {
	CitrusLobby_init(&lobby->lobby);
	lobby->send = send;
	lobby->send_data = send_data;
}

void CitrusServerLobby_client_connect(CitrusServerLobby* lobby, int id) {
	CitrusEvent event;
	event.type = CITRUS_EVENT_CONNECT;
	event.client_id = id;
	CitrusLobby_event(&lobby->lobby, event);
}

void CitrusServerLobby_client_disconnect(CitrusServerLobby* lobby, int id) {
	CitrusEvent event;
	event.type = CITRUS_EVENT_DISCONNECT;
	event.client_id = id;
	CitrusLobby_event(&lobby->lobby, event);
}

void CitrusServerLobby_recv(CitrusServerLobby* lobby, int n, uint8_t* data, int id) {
	if (lobby->lobby.connected_clients[id]) {
		CitrusParser_send(&lobby->parsers[id], n, data);
		CitrusEvent event;
		while (CitrusParser_get_event(&lobby->parsers[id], &event)) {
			CitrusLobby_event(&lobby->lobby, event);
		}
	}
}
