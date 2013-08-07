/*
 * Copyright (C) 2013 - David Goulet <dgoulet@ev0ke.net>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, version 2 only, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <assert.h>

#include <common/log.h>

#include "torsocks.h"

/*
 * Torsocks call for socket(2)
 */
LIBC_SOCKET_RET_TYPE tsocks_socket(LIBC_SOCKET_SIG)
{
	switch (__type) {
	case SOCK_STREAM:
		break;
	default:
		if (__domain == AF_INET || __domain == AF_INET6) {
			ERR("Non TCP socket denied. Tor network can't handle it. "
					"Stopping everything!");
			errno = EINVAL;
			return -1;
		}
		break;
	}

	/* Stream socket for INET/INET6 is good so open it. */
	return tsocks_libc_socket(__domain, __type, __protocol);
}

/*
 * Libc hijacked symbol socket(2).
 */
LIBC_SOCKET_DECL
{
	/* Find symbol if not already set. Exit if not found. */
	tsocks_libc_socket = tsocks_find_libc_symbol(LIBC_SOCKET_NAME_STR,
			TSOCKS_SYM_EXIT_NOT_FOUND);
	return tsocks_socket(LIBC_SOCKET_ARGS);
}
