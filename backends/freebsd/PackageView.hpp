/*
 * Copyright (C) Serenity Cybersecurity, LLC <license@futurecrew.ru>
 *               Author: Gleb Popov <arrowd@FreeBSD.org>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <functional>

#include <pk-backend.h>
#include <pkg.h>

#include "Deleters.hpp"

class PackageView
{
public:
    PackageView(struct pkg* pkg) {
        char* buf;

        pkg_asprintf(&buf, "%n", pkg);
        _name = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%v", pkg);
        _version = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%q", pkg);
        _abi = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%N", pkg);
        _reponame = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%c", pkg);
        _comment = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%L", pkg);
        _licenses = free_deleted_unique_ptr<char>(buf);
        pkg_asprintf(&buf, "%s", pkg);
        _flatsize = std::atoi(buf);
    }

    PackageView(gchar* package_id)
    : external_pk_id (package_id), pk_namever(nullptr) {
        g_assert (pk_package_id_check (package_id));
        pk_id_parts = g_strfreev_deleted_unique_ptr<gchar*> (pk_package_id_split (package_id));
    }

    PackageView(const PackageView&) = delete;

    const gchar* name() const {
        if (pk_id_parts)
            return pk_id_parts.get()[PK_PACKAGE_ID_NAME];
        else
            return _name.get();
    }

    const gchar* version() const {
        if (pk_id_parts)
            return pk_id_parts.get()[PK_PACKAGE_ID_VERSION];
        else
            return _version.get();
    }

    gchar* nameversion() {
        if (pk_namever)
            return pk_namever.get();

        pk_namever = g_free_deleted_unique_ptr<gchar> (g_strconcat(name(), "-", version(), NULL));
        return pk_namever.get();
    }

    const gchar* comment() const {
        // comment can only be obtained from pkg*
        g_assert (pk_id_parts == nullptr);
        return _comment.get();
    }

    const gchar* arch() const {
        if (pk_id_parts)
            return pk_id_parts.get()[PK_PACKAGE_ID_ARCH];
        else {
            // abi has the form of "FreeBSD:13:amd64"
            // we want only the last part
            const gchar* ptr = _abi.get();
            while (*ptr != ':') ptr++;
            ptr++;
            while (*ptr != ':') ptr++;
            ptr++;
            return ptr;
        }
    }

    const gchar* license() const {
        // licenses can only be obtained from pkg*
        g_assert (pk_id_parts == nullptr);
        return _licenses.get();
    }

    int64_t flatsize() const {
        // flatsize can only be obtained from pkg*
        g_assert (pk_id_parts == nullptr);
        return _flatsize;
    }

    const gchar* repository() const {
        if (pk_id_parts)
            return pk_id_parts.get()[PK_PACKAGE_ID_DATA];
        else
            return _reponame.get(); // TODO: we get nullptr here in case of local pkg
    }

    gchar* packageKitId() {
        if (external_pk_id)
            return external_pk_id;

        if (!built_pk_id)
            built_pk_id = g_free_deleted_unique_ptr<gchar> (
                    pk_package_id_build(name(), version(), arch(), repository()));
        return built_pk_id.get();
    }
private:
    free_deleted_unique_ptr<char> _name,
        _version, _abi, _reponame, _comment, _licenses;
    int64_t _flatsize;
    gchar* external_pk_id = nullptr;
    g_free_deleted_unique_ptr<gchar> built_pk_id;
    g_strfreev_deleted_unique_ptr<gchar*> pk_id_parts;
    g_free_deleted_unique_ptr<gchar> pk_namever;
};
