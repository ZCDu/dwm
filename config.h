#include <X11/XF86keysym.h>

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel , tagbar size */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const char *fonts[]          = { "Hack Nerd Font Mono:size=25",
                                      "Noto Emoji Nerd Font Complete Mono:pixelsize=30" };
static const char dmenufont[]       = "Hack Nerd Font Mono:size=25";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#37474F";
static const char col_border[]        = "#FF0000";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_border  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, 0xd0, OPAQUE },
	[SchemeSel]  = { OPAQUE, 0xd0, OPAQUE },
};

/* tagging */
static const char *tags[] = { "一", "二", "三", "四", "五", "六", "七", "八", "九" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window*/

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "Tile",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },                 
	{ "[\\]",     dwindle },                
	{ "H[]",      deck },                   
	{ "TTT",      bstack },                 
	{ "===",      bstackhoriz },            
	{ "HHH",      grid },                   
	{ "###",      nrowgrid },               
	{ "---",      horizgrid },              
	{ ":::",      gaplessgrid },            
	{ "|M|",      centeredmaster },         
	{ ">M>",      centeredfloatingmaster }, 
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
//static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = {"rofi", "-show", "drun", NULL }; 
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[]  = { "google-chrome", NULL };
static const char *firebrowsercmd[]  = { "firefox", NULL };

static const char *upvol[]   = { "/home/zcdu/software/dwmscripts/vol-up.sh",  NULL };
static const char *downvol[] = { "/home/zcdu/software/dwmscripts/vol-down.sh",  NULL };
static const char *mutevol[] = { "/home/zcdu/software/dwmscripts/vol-toggle.sh",  NULL };
static const char *flameshotcmd[] = {"flameshot", "gui", NULL};

static const char *wpcmd[]  = { "/home/zcdu/software/dwmscripts/wp-change.sh", NULL };
static const char *sktogglecmd[]  = { "/home/zcdu/software/dwmscripts/sck-tog.sh", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "80x24", NULL };
static const char *alacrittycmd[] = {"gnome-terminal", NULL};

static const char *suspendcmd[]  = { "/home/zcdu/software/dwmscripts/suspend.sh", NULL };

static Key keys[] = {
	/* modifier            key                      function        argument */
	{ MODKEY,              XK_s,                    spawn,          {.v = dmenucmd } },
	{ MODKEY,              XK_Return,               spawn,          {.v = termcmd } },
	{ MODKEY,              XK_c,                    spawn,          {.v = browsercmd } },
	{ MODKEY,              XK_x,                    spawn,          {.v = firebrowsercmd } },
	{ MODKEY,              XK_a,                    spawn,          {.v = alacrittycmd } },
	{ MODKEY|ControlMask,  XK_s,                    spawn,          {.v = sktogglecmd } },
	{ MODKEY|ShiftMask,    XK_a,                    spawn,          {.v = flameshotcmd } },
	{ 0,                   XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
	{ 0,                   XF86XK_AudioMute,        spawn,          {.v = mutevol } },
	{ 0,                   XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
	{ MODKEY,              XK_bracketleft,          spawn,          {.v = downvol } },
	{ MODKEY,              XK_backslash,            spawn,          {.v = mutevol } },
	{ MODKEY,              XK_bracketright,         spawn,          {.v = upvol   } },
	{ MODKEY,              XK_u,                    spawn,          {.v = wpcmd } },
	{ MODKEY|ShiftMask,    XK_e,                    rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_b,                    rotatestack,    {.i = -1 } },
	{ MODKEY,              XK_e,                    focusstack,     {.i = +1 } },
	{ MODKEY,              XK_b,                    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_h,                    incnmaster,     {.i = +1 } }, // 调整为左右分屏的形式
	{ MODKEY|ShiftMask,    XK_l,                    incnmaster,     {.i = -1 } },
	{ MODKEY,              XK_h,                    setmfact,       {.f = -0.05} },
	{ MODKEY,              XK_l,                    setmfact,       {.f = +0.05} },
	{ MODKEY,              XK_k,                    hidewin,        {0} },
	{ MODKEY|ShiftMask,    XK_k,                    restorewin,     {0} },
	{ MODKEY|ShiftMask,    XK_Return,               zoom,           {0} },
	{ MODKEY,              XK_Tab,                  view,           {0} },
	{ MODKEY|ShiftMask,    XK_q,                    killclient,     {0} },
	{ MODKEY,              XK_t,                    setlayout,      {.v = &layouts[0]} },
	{ MODKEY,              XK_n,                    setlayout,      {.v = &layouts[1]} },
	{ MODKEY,              XK_m,                    setlayout,      {.v = &layouts[2]} },
	{ MODKEY,              XK_r,                    setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,    XK_r,                    setlayout,      {.v = &layouts[12]} },
	{ MODKEY|ShiftMask,    XK_f,                    fullscreen,     {0} },
	{ MODKEY|ShiftMask,    XK_space,                togglefloating, {0} },
	{ MODKEY,              XK_w,                    togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,              XK_0,                    view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,    XK_0,                    tag,            {.ui = ~0 } },
	{ MODKEY,              XK_o,                focusmon,       {.i = -1 } },
	{ MODKEY,              XK_p,               focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,    XK_o,                tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,    XK_p,               tagmon,         {.i = +1 } },
	TAGKEYS(               XK_1,                      0)
	TAGKEYS(               XK_2,                      1)
	TAGKEYS(               XK_3,                      2)
	TAGKEYS(               XK_4,                      3)
	TAGKEYS(               XK_5,                      4)
	TAGKEYS(               XK_6,                      5)
	TAGKEYS(               XK_7,                      6)
	TAGKEYS(               XK_8,                      7)
	TAGKEYS(               XK_9,                      8)
	{ MODKEY|ControlMask,  XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
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

