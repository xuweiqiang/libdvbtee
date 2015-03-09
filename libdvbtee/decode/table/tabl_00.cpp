/*****************************************************************************
 * Copyright (C) 2011-2015 Michael Ira Krufky
 *
 * Author: Michael Ira Krufky <mkrufky@linuxtv.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "decoder.h"
//#include "dvbpsi/descriptor.h"

#include "functions.h"

#include "tabl_00.h"

#define TABLEID 0x00

#define CLASS_MODULE "[PAT]"

#define dprintf(fmt, arg...) __dprintf(DBG_DECODE, fmt, ##arg)

using namespace dvbtee::decode;

static std::string TABLE_NAME = "PAT[00]";

void pat::store(dvbpsi_pat_t *p_pat)
#define PAT_DBG 1
{
#if PAT_DBG
	fprintf(stderr, "%s: v%d, ts_id: %d\n", __func__,
		p_pat->i_version, p_pat->i_ts_id);
#endif
	m_ts_id   = p_pat->i_ts_id;
	m_version = p_pat->i_version;
	m_programs.clear();

	dvbpsi_pat_program_t* p_program = p_pat->p_first_program;
	while (p_program) {
		m_programs[p_program->i_number] = p_program->i_pid;

//		rcvd_pmt[p_program->i_number] = false;
#if PAT_DBG
		fprintf(stderr, "  %10d | %x\n",
			p_program->i_number,
			m_programs[p_program->i_number]);
#endif
		p_program = p_program->p_next;
	}

	if ((/*changed*/true) && (m_watcher)) {
		m_watcher->updateTable(TABLEID, (Table*)this);
	}
}

pat::pat(Decoder *parent)
 : Table(parent, TABLE_NAME, TABLEID)
 , m_ts_id(0xffff)
 , m_version(0xff)
{
	//store table later (probably repeatedly)
}

pat::pat(Decoder *parent, TableWatcher *watcher)
 : Table(parent, TABLE_NAME, TABLEID, watcher)
 , m_ts_id(0xffff)
 , m_version(0xff)
{
	//store table later (probably repeatedly)
}

pat::pat(Decoder *parent, TableWatcher *watcher, dvbpsi_pat_t *p_pat)
 : Table(parent, TABLE_NAME, TABLEID, watcher)
 , m_ts_id(0xffff)
 , m_version(0xff)
{
	store(p_pat);
}

pat::pat(Decoder *parent, dvbpsi_pat_t *p_pat)
 : Table(parent, TABLE_NAME, TABLEID)
 , m_ts_id(0xffff)
 , m_version(0xff)
{
	store(p_pat);
}

pat::~pat()
{
	//
}

REGISTER_TABLE_FACTORY(TABLEID, dvbpsi_pat_t, pat);