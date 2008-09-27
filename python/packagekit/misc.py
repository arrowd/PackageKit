#!/usr/bin/python
#
# Licensed under the GNU General Public License Version 2
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# (c) 2008
#     Tim Lauridsen <timlau@fedoraproject.org>

# Misc classes and funtions

class PackageKitPackage:
    '''
    container class from values from the Package signal
    '''
    def __init__(self, info, package_id, summary):
        self.installed = (info == 'installed')
        self.id = str(package_id)
        self.summary = unicode(summary)
        self.info = str(info)

    def __str__(self):
        (name, ver, arch, repo) = tuple(self.id.split(";"))
        p =  "%s-%s.%s" % (name, ver, arch)
        return "%-40s : %s : %s" % (p, self.info, self.summary)

class PackageKitDistroUpgrade:
    '''
    container class from values from the DistroUpgrade signal
    '''
    def __init__(self, upgrade_type, name, summary):
        self.upgrade_type = upgrade_type
        self.name = name
        self.summary = (summary)

    def __str__(self):
        return " type : %s, name : %s, summary : %s " % (
                self.upgrade_type, self.name, self.summary)

class PackageKitDetails:
    '''
    container class from values from the Detail signal
    '''
    def __init__(self, package_id, package_license, group, detail, url, size):
        self.id = str(package_id)
        self.license = package_license
        self.group = group
        self.detail = unicode(detail)
        self.url = url
        self.size = size

class PackageKitUpdateDetails:
    '''
    container class from values from the UpdateDetail signal
    '''
    def __init__(self, package_id, updates, obsoletes, vendor_url, bugzilla_url, \
                 cve_url, restart, update_text, changelog, state, \
                 issued, updated):
        self.id = str(package_id)
        self.updates = updates
        self.obsoletes = obsoletes
        self.vendor_url = vendor_url
        self.bugzilla_url = bugzilla_url
        self.cve_url = cve_url
        self.restart = restart
        self.update_text = update_text
        self.changelog = changelog
        self.state = state
        self.issued = issued
        self.updated = updated

class PackageKitRepos:
    '''
    container class from values from the Repos signal
    '''
    def __init__(self, package_id, description, enabled):
        self.id = str(package_id)
        self.description = description
        self.enabled = enabled

class PackageKitFiles:
    '''
    container class from values from the Files signal
    '''
    def __init__(self, package_id, files):
        self.id = str(package_id)
        self.files = files
            
