/*
 * Copyright (c) 2013, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "TabSample.h"
#include <sqlpp11/alias_provider.h>
#include <sqlpp11/select.h>
#include <sqlpp11/insert.h>
#include <sqlpp11/update.h>
#include <sqlpp11/remove.h>
#include <sqlpp11/functions.h>
#include <sqlpp11/transaction.h>
#include <sqlpp11/multi_column.h>
#include <sqlpp11/sqlite3/connection.h>

#include <sqlite3.h>
#include <iostream>
#include <vector>


SQLPP_ALIAS_PROVIDER(left);

namespace sql = sqlpp::sqlite3;
int main()
{
	auto config = std::make_shared<sql::connection_config>();
 	config->path_to_database = ":memory:";
	config->flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
	config->debug = true;

	sql::connection db(config);
	db.execute("CREATE TABLE tab_sample (\
		alpha bigint(20) DEFAULT NULL,\
			beta varchar(255) DEFAULT NULL,\
			gamma bool DEFAULT NULL\
			)");

	TabSample tab;

	db.run(insert_into(tab).columns(tab.beta, tab.gamma)
			.add_values(tab.beta = "rhabarbertorte", tab.gamma = false)
			//.add_values(tab.beta = "cheesecake", tab.gamma = false)
			//.add_values(tab.beta = "kaesekuchen", tab.gamma = true)
			);

	// select a static (alpha) and a dynamic column (beta)
	for(const auto& row : db.run(dynamic_select(db).dynamic_columns(tab.alpha).add_column(tab.beta).from(tab).where(true)))
	{
		std::cerr << "row.alpha: " << row.alpha << ", row.beta: " << row.at("beta") << std::endl;
	};
	return 0;
}