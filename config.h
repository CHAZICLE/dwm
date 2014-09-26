/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const char font[]            = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            True,        -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", font, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd_new[]  = { "urxvt", "-e", "tmux-terminal-main", "new", NULL };
static const char *termcmd[]  = { "urxvt", "-e", "tmux-terminal-main", NULL };
static const char *volume_up[] = {"pulseaudio-ctl", "up", "5", NULL};
static const char *volume_down[] = {"pulseaudio-ctl", "down", "5", NULL};
static const char *volume_mute[] = {"pulseaudio-ctl", "mute", NULL};
static const char *screen_br_up[] = {"sudo", "backlight", "screen", "up", NULL};
static const char *screen_br_dn[] = {"sudo", "backlight", "screen", "dn", NULL};
static const char *screen_br_of[] = {"sudo", "backlight", "screen", "off", NULL};
static const char *screen_br_on[] = {"sudo", "backlight", "screen", "on", NULL};
static const char *kboard_br_up[] = {"sudo", "backlight", "keyboard", "up", NULL};
static const char *kboard_br_dn[] = {"sudo", "backlight", "keyboard", "dn", NULL};
static const char *kboard_br_of[] = {"sudo", "backlight", "keyboard", "off", NULL};
static const char *kboard_br_on[] = {"sudo", "backlight", "keyboard", "on", NULL};
static const char *media_prev[] = {"cmus-remote", "--prev", NULL};
static const char *media_paus[] = {"cmus-remote", "--pause", NULL};
static const char *media_next[] = {"cmus-remote", "--next", NULL};
static const char *dpms_screenoff[] = {"bash", "-c", "sleep 1; xset dpms force off", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd_new } },
	{ MODKEY|ShiftMask|ControlMask, XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ Mod4Mask,                       XK_comma,  focusmon,       {.i = -1 } },
	{ Mod4Mask,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ Mod4Mask|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ Mod4Mask|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ 0,				XF86XK_AudioLowerVolume,   spawn,        { .v = volume_down } },
	{ 0,				XF86XK_AudioRaiseVolume,   spawn,        { .v = volume_up } },
	{ 0,				XF86XK_AudioMute,          spawn,        { .v = volume_mute } },
        { 0,                            XF86XK_MonBrightnessUp,    spawn,        { .v = screen_br_up } },
        { 0,                            XF86XK_MonBrightnessDown,  spawn,        { .v = screen_br_dn } },
        { MODKEY,                       XF86XK_MonBrightnessUp,    spawn,        { .v = screen_br_on } },
        { MODKEY,                       XF86XK_MonBrightnessDown,  spawn,        { .v = screen_br_of } },
        { 0,                            XF86XK_KbdBrightnessUp,    spawn,        { .v = kboard_br_up } },
        { 0,                            XF86XK_KbdBrightnessDown,  spawn,        { .v = kboard_br_dn } },
        { MODKEY,                       XF86XK_KbdBrightnessUp,    spawn,        { .v = kboard_br_on } },
        { MODKEY,              	        XF86XK_KbdBrightnessDown,  spawn,        { .v = kboard_br_of } },
        { 0,              	        XF86XK_AudioPrev,	   spawn,        { .v = media_prev } },
        { 0,              	        XF86XK_AudioPlay,	   spawn,        { .v = media_paus } },
        { 0,              	        XF86XK_AudioNext,	   spawn,        { .v = media_next } },
        { 0,              	        XF86XK_Sleep,		   spawn,        { .v = dpms_screenoff } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

