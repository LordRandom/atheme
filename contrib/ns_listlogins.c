/*
 * Copyright (c) 2005-2007 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * This file contains code for the NickServ LISTLOGINS function.
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"nickserv/listlogins", false, _modinit, _moddeinit,
	PACKAGE_STRING,
	"Atheme Development Group <http://www.atheme.org>"
);

static void ns_cmd_listlogins(sourceinfo_t *si, int parc, char *parv[]);

command_t ns_listlogins = { "LISTLOGINS", N_("Lists details of clients authenticated as you."), AC_NONE, 1, ns_cmd_listlogins };

list_t *ns_cmdtree, *ns_helptree;

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(ns_cmdtree, "nickserv/main", "ns_cmdtree");
	MODULE_USE_SYMBOL(ns_helptree, "nickserv/main", "ns_helptree");

	command_add(&ns_listlogins, ns_cmdtree);
	help_addentry(ns_helptree, "LISTLOGINS", "help/contrib/listlogins", NULL);
}

void _moddeinit()
{
	command_delete(&ns_listlogins, ns_cmdtree);
	help_delentry(ns_helptree, "LISTLOGINS");
}

static void ns_cmd_listlogins(sourceinfo_t *si, int parc, char *parv[])
{
	user_t *u;
	node_t *n;
	int matches = 0;

	if (si->smu == NULL)
	{
		command_fail(si, fault_noprivs, _("You are not logged in."));
		return;
	}

	if (si->smu->flags & MU_WAITAUTH)
	{
		command_fail(si, fault_noprivs, _("You have to verify your email address before you can perform this operation."));
		return;
	}

	command_success_nodata(si, "Clients identified to account \2%s\2", entity(si->smu)->name);
	LIST_FOREACH(n, si->smu->logins.head)
	{
		u = n->data;
		command_success_nodata(si, "- %s!%s@%s (real host: %s)", u->nick, u->user, u->vhost, u->host);
		matches++;
	}
	command_success_nodata(si, ngettext(N_("\2%d\2 client found"), N_("\2%d\2 clients found"), matches), matches);
	logcommand(si, CMDLOG_GET, "LISTLOGINS: (\2%d\2 matches)", matches);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */
