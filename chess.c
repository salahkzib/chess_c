/*
 * ═══════════════════════════════════════════════════════════════════
 *  VOID CHESS — Professional Chess Engine & UI in C
 *  Inspired by chess.com / lichess design language
 *  Dependencies: SDL2, SDL2_ttf, SDL2_gfx
 *
 *  Build:
 *    gcc chess.c -o chess \
 *        $(sdl2-config --cflags --libs) \
 *        -lSDL2_ttf -lSDL2_gfx -lm
 *
 *  Install deps (Ubuntu/Debian):
 *    sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-gfx-dev
 *
 *  Install deps (macOS):
 *    brew install sdl2 sdl2_ttf sdl2_gfx
 * ═══════════════════════════════════════════════════════════════════
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

/* ── Window & Layout ──────────────────────────────────────────────── */
#define WINDOW_W        1200
#define WINDOW_H        800
#define BOARD_MARGIN    48
#define BOARD_SIZE      640
#define SQUARE_SIZE     (BOARD_SIZE / 8)
#define SIDEBAR_X       (BOARD_MARGIN + BOARD_SIZE + 32)
#define SIDEBAR_W       (WINDOW_W - SIDEBAR_X - 20)

/* ── Colors (chess.com / dark lichess inspired) ────────────────────── */
/* Background */
#define COL_BG           0x1A1A1Aff
#define COL_PANEL        0x262626ff
#define COL_PANEL2       0x2E2E2Eff
#define COL_BORDER       0x3A3A3Aff

/* Board */
#define COL_LIGHT_SQ     0xEEDEBDff   /* classic cream */
#define COL_DARK_SQ      0xB58863ff   /* warm brown */
#define COL_LIGHT_SQ_HL  0xF6F668ff   /* last move highlight light */
#define COL_DARK_SQ_HL   0xCDD16Aff   /* last move highlight dark */
#define COL_SELECTED     0x20B2AA88   /* teal selection overlay */
#define COL_MOVE_DOT     0x00000044   /* possible move dot */
#define COL_MOVE_RING    0x00000066   /* possible capture ring */
#define COL_CHECK        0xFF4444CCff /* king in check */

/* Text */
#define COL_TEXT_PRI     0xF0F0F0ff
#define COL_TEXT_SEC     0x9A9A9Aff
#define COL_TEXT_ACC     0x81B64Cff   /* green accent */
#define COL_TEXT_COORD   0x9A7B5Aff

/* UI elements */
#define COL_BTN_BG       0x3D3D3Dff
#define COL_BTN_HOVER    0x4A4A4Aff
#define COL_BTN_ACC      0x81B64Cff
#define COL_MOVE_LIST_BG 0x1E1E1Eff
#define COL_MOVE_W       0x333333ff
#define COL_MOVE_B       0x1E1E1Eff
#define COL_MOVE_ACTIVE  0x4A6741ff

/* Animation */
#define ANIM_DURATION_MS  200
#define HOVER_ANIM_MS     80

/* ── Piece definitions ────────────────────────────────────────────── */
typedef enum {
    EMPTY = 0,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
} Piece;

#define IS_WHITE(p)  ((p) >= W_PAWN  && (p) <= W_KING)
#define IS_BLACK(p)  ((p) >= B_PAWN  && (p) <= B_KING)
#define IS_EMPTY(p)  ((p) == EMPTY)
#define PIECE_TYPE(p) (IS_WHITE(p) ? (p) : IS_BLACK(p) ? ((p) - B_PAWN + 1) : 0)

/* ── Move structure ───────────────────────────────────────────────── */
typedef struct {
    int from_row, from_col;
    int to_row,   to_col;
    Piece captured;
    Piece promotion;
    bool is_castle_king;
    bool is_castle_queen;
    bool is_en_passant;
    char notation[16];
} Move;

/* ── Animation state ──────────────────────────────────────────────── */
typedef struct {
    bool   active;
    Piece  piece;
    float  sx, sy;   /* start pixel x,y */
    float  ex, ey;   /* end pixel x,y */
    float  cx, cy;   /* current pixel x,y */
    Uint32 start_ms;
    Uint32 dur_ms;
} PieceAnim;

/* ── Game state ───────────────────────────────────────────────────── */
typedef struct {
    Piece  board[8][8];
    bool   white_turn;
    int    selected_row, selected_col;   /* -1 if none */
    bool   has_selection;
    Move   legal_moves[256];
    int    legal_count;
    Move   move_history[512];
    int    move_count;
    int    last_from_row, last_from_col;
    int    last_to_row,   last_to_col;
    bool   has_last_move;

    /* Castling rights */
    bool   w_castle_k, w_castle_q;
    bool   b_castle_k, b_castle_q;

    /* En passant */
    int    ep_col;   /* -1 if none, else col of pawn that just double-moved */
    int    ep_row;

    /* Check / game over */
    bool   white_in_check, black_in_check;
    bool   game_over;
    char   result_str[64];

    /* UI state */
    int    hover_row, hover_col;
    bool   dragging;
    int    drag_piece_row, drag_piece_col;
    float  drag_x, drag_y;
    bool   flipped;            /* board orientation */
    int    move_list_scroll;

    /* Captured pieces */
    Piece  w_captured[16];
    int    w_cap_count;
    Piece  b_captured[16];
    int    b_cap_count;
    int    material_diff;

    /* Timers (seconds, -1 = untimed) */
    float  w_time, b_time;
    bool   timed_game;
    Uint32 last_tick;

    PieceAnim anim;

    /* Promotion */
    bool   promoting;
    int    promo_row, promo_col;
    int    promo_from_row, promo_from_col;

    /* Button hover states */
    bool   btn_new_hover;
    bool   btn_flip_hover;
    bool   btn_undo_hover;
    bool   btn_resign_hover;
} GameState;

/* ── Globals ──────────────────────────────────────────────────────── */
static SDL_Window   *g_window   = NULL;
static SDL_Renderer *g_renderer = NULL;
static TTF_Font     *g_font_lg  = NULL;   /* 22px  — titles   */
static TTF_Font     *g_font_md  = NULL;   /* 16px  — labels   */
static TTF_Font     *g_font_sm  = NULL;   /* 13px  — notation */
static TTF_Font     *g_font_xs  = NULL;   /* 11px  — coords   */
static TTF_Font     *g_font_pieces = NULL; /* 36px — pieces    */
static GameState     g_state;
static bool          g_running  = true;

/* ── Unicode chess pieces ─────────────────────────────────────────── */
static const char *PIECE_UNICODE[13] = {
    "",
    "♙","♘","♗","♖","♕","♔",   /* white */
    "♟","♞","♝","♜","♛","♚"    /* black */
};

/* FEN-style piece chars for notation */
static const char PIECE_CHAR[7] = { ' ','P','N','B','R','Q','K' };

/* ─────────────────────────────────────────────────────────────────── */
/*  COLOUR HELPERS                                                     */
/* ─────────────────────────────────────────────────────────────────── */
static SDL_Color hex2col(Uint32 hex) {
    SDL_Color c;
    c.r = (hex >> 24) & 0xFF;
    c.g = (hex >> 16) & 0xFF;
    c.b = (hex >>  8) & 0xFF;
    c.a =  hex        & 0xFF;
    return c;
}
static void set_col(Uint32 hex) {
    SDL_Color c = hex2col(hex);
    SDL_SetRenderDrawColor(g_renderer, c.r, c.g, c.b, c.a);
}
static void set_col_a(Uint32 hex, Uint8 alpha) {
    SDL_Color c = hex2col(hex);
    SDL_SetRenderDrawColor(g_renderer, c.r, c.g, c.b, alpha);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  TEXT RENDERING                                                     */
/* ─────────────────────────────────────────────────────────────────── */
static void draw_text(TTF_Font *font, const char *text, int x, int y,
                      Uint32 color_hex, bool center_x, bool center_y) {
    if (!font || !text || text[0] == '\0') return;
    SDL_Color col = hex2col(color_hex);
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text, col);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, surf);
    if (!tex) { SDL_FreeSurface(surf); return; }
    SDL_Rect dst = { x, y, surf->w, surf->h };
    if (center_x) dst.x = x - surf->w / 2;
    if (center_y) dst.y = y - surf->h / 2;
    SDL_RenderCopy(g_renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

static void draw_text_rect(TTF_Font *font, const char *text, SDL_Rect r, Uint32 col) {
    draw_text(font, text, r.x + r.w/2, r.y + r.h/2, col, true, true);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  SHAPE HELPERS                                                      */
/* ─────────────────────────────────────────────────────────────────── */
static void fill_rect(int x,int y,int w,int h, Uint32 col) {
    set_col(col);
    SDL_Rect r={x,y,w,h};
    SDL_RenderFillRect(g_renderer,&r);
}

static void draw_rect_border(int x,int y,int w,int h, Uint32 col, int thickness) {
    set_col(col);
    for(int i=0;i<thickness;i++) {
        SDL_Rect r={x+i,y+i,w-2*i,h-2*i};
        SDL_RenderDrawRect(g_renderer,&r);
    }
}

static void draw_rounded_rect(int x,int y,int w,int h, int r, Uint32 col) {
    SDL_Color c = hex2col(col);
    roundedBoxRGBA(g_renderer, x, y, x+w, y+h, r,
                   c.r, c.g, c.b, c.a);
}

static void draw_rounded_rect_border(int x,int y,int w,int h,int r,Uint32 col,int thick){
    SDL_Color c = hex2col(col);
    for(int i=0;i<thick;i++)
        roundedRectangleRGBA(g_renderer, x+i, y+i, x+w-i, y+h-i, r,
                             c.r, c.g, c.b, c.a);
}

static void draw_circle_filled(int cx,int cy,int rad, Uint32 col) {
    SDL_Color c = hex2col(col);
    filledCircleRGBA(g_renderer, cx, cy, rad, c.r, c.g, c.b, c.a);
}

static void draw_circle_border(int cx,int cy,int rad,int thick, Uint32 col) {
    SDL_Color c = hex2col(col);
    for(int i=0;i<thick;i++)
        circleRGBA(g_renderer, cx, cy, rad-i, c.r, c.g, c.b, c.a);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  BOARD COORDINATE HELPERS                                           */
/* ─────────────────────────────────────────────────────────────────── */
static int board_x(int col) {
    int c = g_state.flipped ? (7 - col) : col;
    return BOARD_MARGIN + c * SQUARE_SIZE;
}
static int board_y(int row) {
    int r = g_state.flipped ? row : (7 - row);
    return BOARD_MARGIN + r * SQUARE_SIZE;
}
static bool screen_to_board(int sx, int sy, int *row, int *col) {
    int bx = sx - BOARD_MARGIN;
    int by = sy - BOARD_MARGIN;
    if (bx < 0 || bx >= BOARD_SIZE || by < 0 || by >= BOARD_SIZE)
        return false;
    int fc = bx / SQUARE_SIZE;
    int fr = by / SQUARE_SIZE;
    *col = g_state.flipped ? (7 - fc) : fc;
    *row = g_state.flipped ? fr : (7 - fr);
    return true;
}

/* ─────────────────────────────────────────────────────────────────── */
/*  CHESS LOGIC                                                        */
/* ─────────────────────────────────────────────────────────────────── */
static bool in_bounds(int r, int c) {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

/* Returns true if square (r,c) is attacked by 'by_white' side */
static bool is_attacked(Piece board[8][8], int r, int c, bool by_white) {
    /* Pawns */
    int pd = by_white ? 1 : -1;
    Piece atk_pawn = by_white ? W_PAWN : B_PAWN;
    if (in_bounds(r-pd,c-1) && board[r-pd][c-1]==atk_pawn) return true;
    if (in_bounds(r-pd,c+1) && board[r-pd][c+1]==atk_pawn) return true;

    /* Knights */
    Piece atk_kn = by_white ? W_KNIGHT : B_KNIGHT;
    int kn[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(int i=0;i<8;i++) {
        int nr=r+kn[i][0], nc=c+kn[i][1];
        if(in_bounds(nr,nc) && board[nr][nc]==atk_kn) return true;
    }

    /* Bishops / Queens (diagonal) */
    Piece atk_b = by_white ? W_BISHOP : B_BISHOP;
    Piece atk_q = by_white ? W_QUEEN  : B_QUEEN;
    int diag[4][2]={{-1,-1},{-1,1},{1,-1},{1,1}};
    for(int d=0;d<4;d++) {
        int nr=r+diag[d][0], nc=c+diag[d][1];
        while(in_bounds(nr,nc)) {
            Piece p=board[nr][nc];
            if(p!=EMPTY) {
                if(p==atk_b||p==atk_q) return true;
                break;
            }
            nr+=diag[d][0]; nc+=diag[d][1];
        }
    }

    /* Rooks / Queens (straight) */
    Piece atk_r = by_white ? W_ROOK : B_ROOK;
    int straight[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
    for(int d=0;d<4;d++) {
        int nr=r+straight[d][0], nc=c+straight[d][1];
        while(in_bounds(nr,nc)) {
            Piece p=board[nr][nc];
            if(p!=EMPTY) {
                if(p==atk_r||p==atk_q) return true;
                break;
            }
            nr+=straight[d][0]; nc+=straight[d][1];
        }
    }

    /* King */
    Piece atk_k = by_white ? W_KING : B_KING;
    for(int dr=-1;dr<=1;dr++) for(int dc=-1;dc<=1;dc++) {
        if(!dr&&!dc) continue;
        int nr=r+dr, nc=c+dc;
        if(in_bounds(nr,nc) && board[nr][nc]==atk_k) return true;
    }
    return false;
}

static bool king_in_check(Piece board[8][8], bool white_king) {
    Piece k = white_king ? W_KING : B_KING;
    for(int r=0;r<8;r++) for(int c=0;c<8;c++)
        if(board[r][c]==k)
            return is_attacked(board, r, c, !white_king);
    return false;
}

/* Build all pseudo-legal + legal moves for current side */
static void add_move(GameState *gs, int fr, int fc, int tr, int tc,
                     Piece cap, Piece promo,
                     bool ck, bool cq, bool ep) {
    Move *m = &gs->legal_moves[gs->legal_count];
    m->from_row=fr; m->from_col=fc;
    m->to_row=tr;   m->to_col=tc;
    m->captured=cap; m->promotion=promo;
    m->is_castle_king=ck; m->is_castle_queen=cq;
    m->is_en_passant=ep;
    memset(m->notation,0,sizeof(m->notation));
    gs->legal_count++;
}

/* Try move on temp board; return true if king NOT in check after */
static bool move_is_legal(GameState *gs, Move *m) {
    Piece tmp[8][8];
    memcpy(tmp, gs->board, sizeof(tmp));

    /* Apply move on tmp */
    Piece moving = tmp[m->from_row][m->from_col];
    tmp[m->to_row][m->to_col] = m->promotion ? m->promotion : moving;
    tmp[m->from_row][m->from_col] = EMPTY;

    /* En passant capture */
    if(m->is_en_passant) {
        int cap_row = gs->white_turn ? m->to_row-1 : m->to_row+1;
        tmp[cap_row][m->to_col] = EMPTY;
    }
    /* Castling rook */
    if(m->is_castle_king) {
        tmp[m->to_row][5] = tmp[m->to_row][7];
        tmp[m->to_row][7] = EMPTY;
    }
    if(m->is_castle_queen) {
        tmp[m->to_row][3] = tmp[m->to_row][0];
        tmp[m->to_row][0] = EMPTY;
    }

    return !king_in_check(tmp, gs->white_turn);
}

static void generate_legal_moves(GameState *gs) {
    gs->legal_count = 0;
    bool white = gs->white_turn;

    for(int r=0;r<8;r++) for(int c=0;c<8;c++) {
        Piece p = gs->board[r][c];
        if(IS_EMPTY(p)) continue;
        if(white && !IS_WHITE(p)) continue;
        if(!white && !IS_BLACK(p)) continue;

        /* Pawns */
        if(p==W_PAWN || p==B_PAWN) {
            int dir = (p==W_PAWN) ? 1 : -1;
            int start_row = (p==W_PAWN) ? 1 : 6;
            int promo_row = (p==W_PAWN) ? 7 : 0;
            /* Forward one */
            int nr=r+dir;
            if(in_bounds(nr,c) && gs->board[nr][c]==EMPTY) {
                if(nr==promo_row) {
                    Piece promos[4]={W_QUEEN,W_ROOK,W_BISHOP,W_KNIGHT};
                    if(!white) { promos[0]=B_QUEEN;promos[1]=B_ROOK;promos[2]=B_BISHOP;promos[3]=B_KNIGHT; }
                    for(int i=0;i<4;i++) {
                        Move mv={r,c,nr,c,EMPTY,promos[i],0,0,0};
                        if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,c,EMPTY,promos[i],0,0,0);
                    }
                } else {
                    Move mv={r,c,nr,c,EMPTY,EMPTY,0,0,0};
                    if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,c,EMPTY,EMPTY,0,0,0);
                }
                /* Forward two from start */
                if(r==start_row && gs->board[nr+dir][c]==EMPTY) {
                    Move mv={r,c,nr+dir,c,EMPTY,EMPTY,0,0,0};
                    if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr+dir,c,EMPTY,EMPTY,0,0,0);
                }
            }
            /* Captures */
            for(int dc=-1;dc<=1;dc+=2) {
                int nc=c+dc;
                if(!in_bounds(nr,nc)) continue;
                Piece target=gs->board[nr][nc];
                bool can_cap = white ? IS_BLACK(target) : IS_WHITE(target);
                /* En passant */
                bool ep=false;
                if(!can_cap && gs->ep_col==nc && nr==gs->ep_row) {
                    can_cap=true; ep=true;
                }
                if(can_cap) {
                    if(nr==promo_row && !ep) {
                        Piece promos[4]={W_QUEEN,W_ROOK,W_BISHOP,W_KNIGHT};
                        if(!white){promos[0]=B_QUEEN;promos[1]=B_ROOK;promos[2]=B_BISHOP;promos[3]=B_KNIGHT;}
                        for(int i=0;i<4;i++) {
                            Move mv={r,c,nr,nc,target,promos[i],0,0,0};
                            if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,nc,target,promos[i],0,0,0);
                        }
                    } else {
                        Move mv={r,c,nr,nc,target,EMPTY,0,0,ep};
                        if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,nc,target,EMPTY,0,0,ep);
                    }
                }
            }
        }

        /* Knights */
        if(p==W_KNIGHT||p==B_KNIGHT) {
            int kn[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
            for(int i=0;i<8;i++) {
                int nr=r+kn[i][0], nc=c+kn[i][1];
                if(!in_bounds(nr,nc)) continue;
                Piece t=gs->board[nr][nc];
                if((white&&IS_WHITE(t))||(!white&&IS_BLACK(t))) continue;
                Move mv={r,c,nr,nc,t,EMPTY,0,0,0};
                if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,nc,t,EMPTY,0,0,0);
            }
        }

        /* Sliding pieces */
        bool is_bishop = (p==W_BISHOP||p==B_BISHOP);
        bool is_rook   = (p==W_ROOK  ||p==B_ROOK);
        bool is_queen  = (p==W_QUEEN ||p==B_QUEEN);
        if(is_bishop||is_rook||is_queen) {
            int dirs[8][2]={{-1,-1},{-1,1},{1,-1},{1,1},{-1,0},{1,0},{0,-1},{0,1}};
            int start = (is_bishop)?0:(is_rook)?4:0;
            int end   = (is_bishop)?4:(is_rook)?8:8;
            for(int d=start;d<end;d++) {
                int nr=r+dirs[d][0], nc=c+dirs[d][1];
                while(in_bounds(nr,nc)) {
                    Piece t=gs->board[nr][nc];
                    if((white&&IS_WHITE(t))||(!white&&IS_BLACK(t))) break;
                    Move mv={r,c,nr,nc,t,EMPTY,0,0,0};
                    if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,nc,t,EMPTY,0,0,0);
                    if(t!=EMPTY) break;
                    nr+=dirs[d][0]; nc+=dirs[d][1];
                }
            }
        }

        /* King */
        if(p==W_KING||p==B_KING) {
            for(int dr=-1;dr<=1;dr++) for(int dc=-1;dc<=1;dc++) {
                if(!dr&&!dc) continue;
                int nr=r+dr, nc=c+dc;
                if(!in_bounds(nr,nc)) continue;
                Piece t=gs->board[nr][nc];
                if((white&&IS_WHITE(t))||(!white&&IS_BLACK(t))) continue;
                Move mv={r,c,nr,nc,t,EMPTY,0,0,0};
                if(move_is_legal(gs,&mv)) add_move(gs,r,c,nr,nc,t,EMPTY,0,0,0);
            }
            /* Castling */
            if(white && r==0 && c==4) {
                if(gs->w_castle_k &&
                   gs->board[0][5]==EMPTY && gs->board[0][6]==EMPTY &&
                   !is_attacked(gs->board,0,4,false) &&
                   !is_attacked(gs->board,0,5,false) &&
                   !is_attacked(gs->board,0,6,false)) {
                    Move mv={0,4,0,6,EMPTY,EMPTY,true,false,false};
                    if(move_is_legal(gs,&mv)) add_move(gs,0,4,0,6,EMPTY,EMPTY,true,false,false);
                }
                if(gs->w_castle_q &&
                   gs->board[0][1]==EMPTY && gs->board[0][2]==EMPTY && gs->board[0][3]==EMPTY &&
                   !is_attacked(gs->board,0,4,false) &&
                   !is_attacked(gs->board,0,3,false) &&
                   !is_attacked(gs->board,0,2,false)) {
                    Move mv={0,4,0,2,EMPTY,EMPTY,false,true,false};
                    if(move_is_legal(gs,&mv)) add_move(gs,0,4,0,2,EMPTY,EMPTY,false,true,false);
                }
            }
            if(!white && r==7 && c==4) {
                if(gs->b_castle_k &&
                   gs->board[7][5]==EMPTY && gs->board[7][6]==EMPTY &&
                   !is_attacked(gs->board,7,4,true) &&
                   !is_attacked(gs->board,7,5,true) &&
                   !is_attacked(gs->board,7,6,true)) {
                    Move mv={7,4,7,6,EMPTY,EMPTY,true,false,false};
                    if(move_is_legal(gs,&mv)) add_move(gs,7,4,7,6,EMPTY,EMPTY,true,false,false);
                }
                if(gs->b_castle_q &&
                   gs->board[7][1]==EMPTY && gs->board[7][2]==EMPTY && gs->board[7][3]==EMPTY &&
                   !is_attacked(gs->board,7,4,true) &&
                   !is_attacked(gs->board,7,3,true) &&
                   !is_attacked(gs->board,7,2,true)) {
                    Move mv={7,4,7,2,EMPTY,EMPTY,false,true,false};
                    if(move_is_legal(gs,&mv)) add_move(gs,7,4,7,2,EMPTY,EMPTY,false,true,false);
                }
            }
        }
    }
}

/* Build move notation string */
static void build_notation(GameState *gs, Move *m) {
    Piece p = gs->board[m->from_row][m->from_col];
    int type = PIECE_TYPE(p);
    char buf[16];
    char cols[] = "abcdefgh";
    char rows[] = "12345678";

    if(m->is_castle_king)  { strcpy(m->notation,"O-O");   return; }
    if(m->is_castle_queen) { strcpy(m->notation,"O-O-O"); return; }

    int i = 0;
    if(type != 1) buf[i++] = PIECE_CHAR[type]; /* not pawn */
    if(m->captured != EMPTY) {
        if(type==1) buf[i++] = cols[m->from_col];
        buf[i++] = 'x';
    }
    buf[i++] = cols[m->to_col];
    buf[i++] = rows[m->to_row];
    if(m->promotion) {
        buf[i++] = '=';
        buf[i++] = PIECE_CHAR[PIECE_TYPE(m->promotion)];
    }
    buf[i] = '\0';
    strcpy(m->notation, buf);
}

/* Apply a move */
static void apply_move(GameState *gs, Move *m) {
    build_notation(gs, m);
    Piece moving = gs->board[m->from_row][m->from_col];

    /* Track captured for material */
    if(m->captured != EMPTY) {
        if(IS_WHITE(m->captured)) {
            gs->b_captured[gs->b_cap_count++] = m->captured;
            gs->material_diff -= 1; /* simplified */
        } else {
            gs->w_captured[gs->w_cap_count++] = m->captured;
            gs->material_diff += 1;
        }
    }

    /* Set up animation */
    gs->anim.active   = true;
    gs->anim.piece    = moving;
    gs->anim.sx       = (float)(board_x(m->from_col) + SQUARE_SIZE/2);
    gs->anim.sy       = (float)(board_y(m->from_row) + SQUARE_SIZE/2);
    gs->anim.ex       = (float)(board_x(m->to_col)   + SQUARE_SIZE/2);
    gs->anim.ey       = (float)(board_y(m->to_row)   + SQUARE_SIZE/2);
    gs->anim.cx       = gs->anim.sx;
    gs->anim.cy       = gs->anim.sy;
    gs->anim.start_ms = SDL_GetTicks();
    gs->anim.dur_ms   = ANIM_DURATION_MS;

    /* Apply board */
    gs->board[m->to_row][m->to_col] = m->promotion ? m->promotion : moving;
    gs->board[m->from_row][m->from_col] = EMPTY;

    if(m->is_en_passant) {
        int cr = gs->white_turn ? m->to_row-1 : m->to_row+1;
        gs->board[cr][m->to_col] = EMPTY;
    }
    if(m->is_castle_king) {
        gs->board[m->to_row][5] = gs->board[m->to_row][7];
        gs->board[m->to_row][7] = EMPTY;
    }
    if(m->is_castle_queen) {
        gs->board[m->to_row][3] = gs->board[m->to_row][0];
        gs->board[m->to_row][0] = EMPTY;
    }

    /* Update castling rights */
    if(moving==W_KING){ gs->w_castle_k=false; gs->w_castle_q=false; }
    if(moving==B_KING){ gs->b_castle_k=false; gs->b_castle_q=false; }
    if(moving==W_ROOK){ if(m->from_col==7)gs->w_castle_k=false; if(m->from_col==0)gs->w_castle_q=false; }
    if(moving==B_ROOK){ if(m->from_col==7)gs->b_castle_k=false; if(m->from_col==0)gs->b_castle_q=false; }

    /* En passant target */
    gs->ep_col = -1;
    if((moving==W_PAWN||moving==B_PAWN) && abs(m->to_row-m->from_row)==2) {
        gs->ep_col = m->to_col;
        gs->ep_row = (m->from_row + m->to_row) / 2 + (gs->white_turn?0:0);
        gs->ep_row = gs->white_turn ? m->from_row+1 : m->from_row-1;
    }

    /* Record move */
    gs->move_history[gs->move_count++] = *m;
    gs->last_from_row=m->from_row; gs->last_from_col=m->from_col;
    gs->last_to_row=m->to_row;     gs->last_to_col=m->to_col;
    gs->has_last_move=true;

    /* Switch turn */
    gs->white_turn = !gs->white_turn;
    gs->has_selection=false;
    gs->selected_row=-1; gs->selected_col=-1;

    /* Check / mate detection */
    generate_legal_moves(gs);
    gs->white_in_check = king_in_check(gs->board, true);
    gs->black_in_check = king_in_check(gs->board, false);

    bool cur_check = gs->white_turn ? gs->white_in_check : gs->black_in_check;

    if(gs->legal_count==0) {
        gs->game_over=true;
        if(cur_check) {
            strcpy(gs->result_str, gs->white_turn ? "0-1  Black wins by checkmate!" : "1-0  White wins by checkmate!");
        } else {
            strcpy(gs->result_str, "½-½  Stalemate!");
        }
    }
    if(cur_check) {
        /* Append '+' or '#' */
        char *n = gs->move_history[gs->move_count-1].notation;
        strcat(n, gs->game_over ? "#" : "+");
    }

    if(gs->timed_game) gs->last_tick = SDL_GetTicks();
}

/* ─────────────────────────────────────────────────────────────────── */
/*  BOARD INITIALIZATION                                               */
/* ─────────────────────────────────────────────────────────────────── */
static void init_board(GameState *gs) {
    memset(gs->board, 0, sizeof(gs->board));
    /* White */
    gs->board[0][0]=W_ROOK; gs->board[0][1]=W_KNIGHT; gs->board[0][2]=W_BISHOP;
    gs->board[0][3]=W_QUEEN; gs->board[0][4]=W_KING;
    gs->board[0][5]=W_BISHOP; gs->board[0][6]=W_KNIGHT; gs->board[0][7]=W_ROOK;
    for(int c=0;c<8;c++) gs->board[1][c]=W_PAWN;
    /* Black */
    gs->board[7][0]=B_ROOK; gs->board[7][1]=B_KNIGHT; gs->board[7][2]=B_BISHOP;
    gs->board[7][3]=B_QUEEN; gs->board[7][4]=B_KING;
    gs->board[7][5]=B_BISHOP; gs->board[7][6]=B_KNIGHT; gs->board[7][7]=B_ROOK;
    for(int c=0;c<8;c++) gs->board[6][c]=B_PAWN;
}

static void new_game(GameState *gs) {
    memset(gs, 0, sizeof(GameState));
    init_board(gs);
    gs->white_turn = true;
    gs->selected_row = gs->selected_col = -1;
    gs->hover_row = gs->hover_col = -1;
    gs->ep_col = -1;
    gs->w_castle_k = gs->w_castle_q = true;
    gs->b_castle_k = gs->b_castle_q = true;
    gs->w_time = 600.0f;
    gs->b_time = 600.0f;
    gs->timed_game = true;
    gs->last_tick = SDL_GetTicks();
    generate_legal_moves(gs);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  DRAWING: BOARD                                                     */
/* ─────────────────────────────────────────────────────────────────── */
static void draw_board(void) {
    GameState *gs = &g_state;

    /* Board shadow */
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    set_col_a(0x00000088, 80);
    for(int i=0;i<8;i++) {
        SDL_Rect sh={BOARD_MARGIN-i+6, BOARD_MARGIN-i+6, BOARD_SIZE+2*i, BOARD_SIZE+2*i};
        SDL_RenderFillRect(g_renderer,&sh);
    }

    /* Squares */
    for(int row=0;row<8;row++) {
        for(int col=0;col<8;col++) {
            int sx = board_x(col);
            int sy = board_y(row);
            bool light = (row+col)%2==0;

            /* Base color */
            Uint32 sq_col;
            if(gs->has_last_move &&
               ((row==gs->last_from_row&&col==gs->last_from_col)||
                (row==gs->last_to_row  &&col==gs->last_to_col))) {
                sq_col = light ? COL_LIGHT_SQ_HL : COL_DARK_SQ_HL;
            } else {
                sq_col = light ? COL_LIGHT_SQ : COL_DARK_SQ;
            }
            fill_rect(sx, sy, SQUARE_SIZE, SQUARE_SIZE, sq_col);

            /* Check highlight */
            bool wc = (gs->white_in_check && gs->board[row][col]==W_KING);
            bool bc = (gs->black_in_check && gs->board[row][col]==B_KING);
            if(wc||bc) {
                SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);
                /* Radial-ish gradient: multiple rects with decreasing alpha */
                for(int r=SQUARE_SIZE/2;r>0;r-=4) {
                    int cx2=sx+SQUARE_SIZE/2, cy2=sy+SQUARE_SIZE/2;
                    Uint8 a=(Uint8)(180*(1.0f-(float)r/(SQUARE_SIZE/2)));
                    SDL_SetRenderDrawColor(g_renderer,255,50,50,a);
                    SDL_Rect rr={cx2-r,cy2-r,2*r,2*r};
                    SDL_RenderFillRect(g_renderer,&rr);
                }
            }

            /* Selected highlight */
            if(gs->has_selection && row==gs->selected_row && col==gs->selected_col) {
                SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(g_renderer,32,178,170,110);
                SDL_Rect hr={sx,sy,SQUARE_SIZE,SQUARE_SIZE};
                SDL_RenderFillRect(g_renderer,&hr);
            }

            /* Hover highlight */
            if(row==gs->hover_row && col==gs->hover_col && !gs->dragging) {
                SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);
                SDL_SetRenderDrawColor(g_renderer,255,255,255,18);
                SDL_Rect hr={sx,sy,SQUARE_SIZE,SQUARE_SIZE};
                SDL_RenderFillRect(g_renderer,&hr);
            }
        }
    }

    /* Possible move dots / rings */
    if(gs->has_selection) {
        SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);
        for(int i=0;i<gs->legal_count;i++) {
            Move *m=&gs->legal_moves[i];
            if(m->from_row!=gs->selected_row||m->from_col!=gs->selected_col) continue;
            int sx=board_x(m->to_col)+SQUARE_SIZE/2;
            int sy=board_y(m->to_row)+SQUARE_SIZE/2;
            bool is_cap = gs->board[m->to_row][m->to_col]!=EMPTY || m->is_en_passant;
            if(is_cap) {
                /* Ring for captures */
                draw_circle_border(sx,sy,SQUARE_SIZE/2-3,5,0x00000055);
            } else {
                /* Dot for quiet moves */
                draw_circle_filled(sx,sy,SQUARE_SIZE/7,0x00000055);
            }
        }
    }

    /* Coordinates */
    char cols[]="abcdefgh", rows_c[]="12345678";
    for(int i=0;i<8;i++) {
        int vi = gs->flipped ? i : (7-i);
        /* Row numbers left */
        char rn[2]={rows_c[vi],'\0'};
        bool light_edge=(vi+0)%2==0;
        Uint32 coord_col = light_edge ? 0xB58863ff : 0xEEDEBDff;
        draw_text(g_font_xs, rn,
                  BOARD_MARGIN+3, BOARD_MARGIN+i*SQUARE_SIZE+5,
                  coord_col, false, false);

        /* Column letters bottom */
        int hi = gs->flipped ? (7-i) : i;
        char cn[2]={cols[hi],'\0'};
        bool light_edge2=(0+hi)%2==0;
        coord_col = light_edge2 ? 0xB58863ff : 0xEEDEBDff;
        draw_text(g_font_xs, cn,
                  BOARD_MARGIN+i*SQUARE_SIZE+SQUARE_SIZE-12,
                  BOARD_MARGIN+BOARD_SIZE-16,
                  coord_col, false, false);
    }
}

/* ─────────────────────────────────────────────────────────────────── */
/*  DRAWING: PIECES                                                    */
/* ─────────────────────────────────────────────────────────────────── */
static void draw_piece_at(Piece p, int px, int py, int size, float alpha) {
    if(IS_EMPTY(p)) return;
    /* Shadow */
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
    SDL_Color sc = hex2col(0x00000000 | (int)(40*alpha));
    filledCircleRGBA(g_renderer, px+3, py+size/2+4, size/2-6,
                     sc.r, sc.g, sc.b, (Uint8)(40*alpha));

    /* Piece unicode glyph */
    SDL_Color col;
    if(IS_WHITE(p)) col = hex2col(0xFFFAF0ff);
    else            col = hex2col(0x1A1A1Aff);

    SDL_Surface *surf = TTF_RenderUTF8_Blended(g_font_pieces, PIECE_UNICODE[p], col);
    if(!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(g_renderer, surf);
    if(!tex) { SDL_FreeSurface(surf); return; }
    SDL_SetTextureAlphaMod(tex, (Uint8)(255*alpha));

    /* Outline for visibility */
    SDL_Color out_col = IS_WHITE(p) ? hex2col(0x4A3728ff) : hex2col(0xE8D5B7ff);
    SDL_Surface *out_surf = TTF_RenderUTF8_Blended(g_font_pieces, PIECE_UNICODE[p], out_col);
    if(out_surf) {
        SDL_Texture *out_tex = SDL_CreateTextureFromSurface(g_renderer, out_surf);
        if(out_tex) {
            SDL_SetTextureAlphaMod(out_tex, (Uint8)(180*alpha));
            for(int dx=-1;dx<=1;dx++) for(int dy=-1;dy<=1;dy++) {
                if(!dx&&!dy) continue;
                SDL_Rect odr={px+dx+2,py+dy+2,surf->w,surf->h};
                SDL_RenderCopy(g_renderer,out_tex,NULL,&odr);
            }
            SDL_DestroyTexture(out_tex);
        }
        SDL_FreeSurface(out_surf);
    }

    SDL_Rect dst={px+2, py+2, surf->w, surf->h};
    SDL_RenderCopy(g_renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
}

static void draw_pieces(void) {
    GameState *gs=&g_state;
    SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);

    for(int row=0;row<8;row++) {
        for(int col=0;col<8;col++) {
            Piece p=gs->board[row][col];
            if(IS_EMPTY(p)) continue;
            /* Skip dragged piece */
            if(gs->dragging && row==gs->drag_piece_row && col==gs->drag_piece_col) continue;
            /* Skip animated piece */
            if(gs->anim.active && gs->anim.piece==p) {
                /* still draw at destination if anim just started and board already updated */
            }
            int sx=board_x(col), sy=board_y(row);
            draw_piece_at(p, sx, sy, SQUARE_SIZE, 1.0f);
        }
    }

    /* Animated piece */
    if(gs->anim.active) {
        Uint32 now=SDL_GetTicks();
        float t=(float)(now-gs->anim.start_ms)/(float)gs->anim.dur_ms;
        if(t>=1.0f) {
            t=1.0f; gs->anim.active=false;
        }
        /* Ease out cubic */
        float et=1.0f-(1.0f-t)*(1.0f-t)*(1.0f-t);
        gs->anim.cx=gs->anim.sx+(gs->anim.ex-gs->anim.sx)*et;
        gs->anim.cy=gs->anim.sy+(gs->anim.ey-gs->anim.sy)*et;
        /* Arc: slight lift */
        float lift=(float)sin(et*3.14159f)*8.0f;
        draw_piece_at(gs->anim.piece,
                      (int)gs->anim.cx-SQUARE_SIZE/2,
                      (int)gs->anim.cy-SQUARE_SIZE/2-(int)lift,
                      SQUARE_SIZE, 1.0f);
    }

    /* Dragged piece */
    if(gs->dragging) {
        Piece p=gs->board[gs->drag_piece_row][gs->drag_piece_col];
        draw_piece_at(p,
                      (int)gs->drag_x-SQUARE_SIZE/2,
                      (int)gs->drag_y-SQUARE_SIZE/2,
                      SQUARE_SIZE, 0.92f);
    }
}

/* ─────────────────────────────────────────────────────────────────── */
/*  DRAWING: PROMOTION PICKER                                          */
/* ─────────────────────────────────────────────────────────────────── */
static void draw_promotion_picker(void) {
    GameState *gs=&g_state;
    if(!gs->promoting) return;

    bool white=!gs->white_turn; /* we just switched */
    Piece choices[4];
    if(white) { choices[0]=W_QUEEN;choices[1]=W_ROOK;choices[2]=W_BISHOP;choices[3]=W_KNIGHT; }
    else      { choices[0]=B_QUEEN;choices[1]=B_ROOK;choices[2]=B_BISHOP;choices[3]=B_KNIGHT; }

    int px=board_x(gs->promo_col);
    int base_y= white ? board_y(gs->promo_row) : board_y(gs->promo_row)-3*SQUARE_SIZE;

    /* Panel background */
    draw_rounded_rect(px-4,base_y-4,SQUARE_SIZE+8,4*SQUARE_SIZE+8,6,0x1E1E1Eff);
    draw_rounded_rect_border(px-4,base_y-4,SQUARE_SIZE+8,4*SQUARE_SIZE+8,6,0x81B64Cff,2);

    for(int i=0;i<4;i++) {
        int sy=base_y+i*SQUARE_SIZE;
        bool light2=(gs->promo_row+gs->promo_col)%2==0;
        fill_rect(px,sy,SQUARE_SIZE,SQUARE_SIZE,light2?0xF0D9B5ff:0xB58863ff);
        draw_piece_at(choices[i],px,sy,SQUARE_SIZE,1.0f);
    }
}

/* ─────────────────────────────────────────────────────────────────── */
/*  DRAWING: SIDEBAR                                                   */
/* ─────────────────────────────────────────────────────────────────── */

/* Time format: mm:ss */
static void fmt_time(float secs, char *buf) {
    int m=(int)(secs/60), s=(int)secs%60;
    if(secs<0){m=0;s=0;}
    sprintf(buf,"%02d:%02d",m,s);
}

static void draw_button(int x,int y,int w,int h,const char *label,
                        bool hover, bool accent) {
    Uint32 bg = hover ? (accent?0x6FAA3Aff:COL_BTN_HOVER) : (accent?COL_BTN_ACC:COL_BTN_BG);
    draw_rounded_rect(x,y,w,h,6,bg);
    if(hover) draw_rounded_rect_border(x,y,w,h,6,accent?0x8ECC4Aff:0x5A5A5Aff,1);
    draw_text_rect(g_font_sm,label,
                   (SDL_Rect){x,y,w,h},
                   accent?0x1A1A1Aff:COL_TEXT_PRI);
}

static void draw_timer_box(int x,int y,int w,int h,
                            float secs,bool active,bool is_white) {
    Uint32 bg = active ? 0xF5F5F5ff : 0x1E1E1Eff;
    Uint32 fg = active ? 0x1A1A1Aff : 0x9A9A9Aff;
    if(secs<30&&active) { bg=0xFF4444ff; fg=0xFFFFFFff; }
    draw_rounded_rect(x,y,w,h,8,bg);
    char tbuf[16]; fmt_time(secs,tbuf);
    draw_text(g_font_lg,tbuf,x+w/2,y+h/2,fg,true,true);
}

static void draw_captured_row(int x,int y,Piece *caps,int count,bool white_caps) {
    /* Sort by piece value */
    static int order[]={W_PAWN,W_PAWN,W_KNIGHT,W_BISHOP,W_ROOK,W_QUEEN};
    (void)order;
    int off=0;
    for(int i=0;i<count;i++) {
        draw_text(g_font_sm,PIECE_UNICODE[caps[i]],
                  x+off,y,white_caps?0xF0F0F0ff:0x555555ff,false,true);
        off+=16;
        if(off>SIDEBAR_W-20) break;
    }
}

static void draw_move_list(void) {
    GameState *gs=&g_state;
    int list_x=SIDEBAR_X, list_y=340;
    int list_w=SIDEBAR_W,  list_h=280;

    /* Background */
    draw_rounded_rect(list_x,list_y,list_w,list_h,8,COL_MOVE_LIST_BG);
    draw_rounded_rect_border(list_x,list_y,list_w,list_h,8,COL_BORDER,1);

    /* Header */
    fill_rect(list_x,list_y,list_w,28,0x252525ff);
    draw_text(g_font_sm,"Moves",list_x+10,list_y+14,COL_TEXT_SEC,false,true);

    /* Clip rendering region */
    int row_h=26, inner_y=list_y+30;
    int visible_rows=(list_h-32)/row_h;
    int total_pairs=(gs->move_count+1)/2;
    int scroll=gs->move_list_scroll;
    if(scroll<0)scroll=0;
    if(scroll>total_pairs-visible_rows&&total_pairs>visible_rows)
        scroll=total_pairs-visible_rows;

    SDL_Rect clip={list_x,inner_y,list_w,list_h-32};
    SDL_RenderSetClipRect(g_renderer,&clip);

    for(int pair=scroll;pair<total_pairs&&pair<scroll+visible_rows+1;pair++) {
        int draw_y=inner_y+(pair-scroll)*row_h;
        bool is_active=(pair==total_pairs-1);
        Uint32 bg=is_active?COL_MOVE_ACTIVE:((pair%2==0)?COL_MOVE_W:COL_MOVE_B);
        fill_rect(list_x,draw_y,list_w,row_h,bg);

        /* Move number */
        char num[8]; sprintf(num,"%d.",pair+1);
        draw_text(g_font_sm,num,list_x+8,draw_y+row_h/2,COL_TEXT_SEC,false,true);

        /* White move */
        int wi=pair*2;
        if(wi<gs->move_count)
            draw_text(g_font_sm,gs->move_history[wi].notation,
                      list_x+48,draw_y+row_h/2,COL_TEXT_PRI,false,true);

        /* Black move */
        int bi=pair*2+1;
        if(bi<gs->move_count)
            draw_text(g_font_sm,gs->move_history[bi].notation,
                      list_x+110,draw_y+row_h/2,COL_TEXT_PRI,false,true);
    }
    SDL_RenderSetClipRect(g_renderer,NULL);

    /* Scroll indicator */
    if(total_pairs>visible_rows) {
        float frac=(float)scroll/(total_pairs-visible_rows);
        int bar_h=(int)((float)visible_rows/total_pairs*(list_h-32));
        int bar_y=inner_y+(int)(frac*(list_h-32-bar_h));
        fill_rect(list_x+list_w-6,bar_y,4,bar_h,0x555555ff);
    }
}

static void draw_sidebar(void) {
    GameState *gs=&g_state;
    int sx=SIDEBAR_X;

    /* Panel background */
    draw_rounded_rect(sx,0,SIDEBAR_W,WINDOW_H,0,COL_PANEL);

    /* ── Player info: Black (top) ── */
    int player_h=60;
    draw_rounded_rect(sx,10,SIDEBAR_W,player_h,8,COL_PANEL2);
    /* Avatar circle */
    draw_circle_filled(sx+30,10+player_h/2,20,0x3A3A3Aff);
    draw_circle_border(sx+30,10+player_h/2,20,2,0x555555ff);
    draw_text(g_font_md,"B",sx+30,10+player_h/2,0xDDDDDDff,true,true);
    /* Name */
    draw_text(g_font_md,"Black",sx+60,10+18,COL_TEXT_PRI,false,false);
    draw_text(g_font_xs,"1500 ELO",sx+60,10+34,COL_TEXT_SEC,false,false);

    /* Black captured pieces */
    draw_captured_row(sx+60,10+50,gs->w_captured,gs->w_cap_count,false);

    /* Black timer */
    bool black_active=!gs->white_turn&&!gs->game_over;
    draw_timer_box(sx+SIDEBAR_W-80,10+player_h/2-18,74,36,
                   gs->b_time,black_active,false);

    /* ── Board area spacer ── */

    /* ── Player info: White (bottom equiv.) ── */
    int wp_y=WINDOW_H-10-player_h;
    draw_rounded_rect(sx,wp_y,SIDEBAR_W,player_h,8,COL_PANEL2);
    draw_circle_filled(sx+30,wp_y+player_h/2,20,0x5A5A5Aff);
    draw_circle_border(sx+30,wp_y+player_h/2,20,2,0x777777ff);
    draw_text(g_font_md,"W",sx+30,wp_y+player_h/2,0xF0F0F0ff,true,true);
    draw_text(g_font_md,"White",sx+60,wp_y+18,COL_TEXT_PRI,false,false);
    draw_text(g_font_xs,"1500 ELO",sx+60,wp_y+34,COL_TEXT_SEC,false,false);

    /* White captured pieces */
    draw_captured_row(sx+60,wp_y+50,gs->b_captured,gs->b_cap_count,true);

    /* White timer */
    bool white_active=gs->white_turn&&!gs->game_over;
    draw_timer_box(sx+SIDEBAR_W-80,wp_y+player_h/2-18,74,36,
                   gs->w_time,white_active,true);

    /* Material advantage */
    if(gs->material_diff!=0) {
        char adv[16];
        sprintf(adv,"%s+%d", gs->material_diff>0?"W":"B",
                abs(gs->material_diff));
        int ay= gs->material_diff>0 ? wp_y-20 : 80;
        draw_text(g_font_xs,adv,sx+SIDEBAR_W-50,ay,COL_TEXT_ACC,true,true);
    }

    /* ── Move list ── */
    draw_move_list();

    /* ── Buttons ── */
    int btn_y=640, btn_h=36, btn_w=(SIDEBAR_W-30)/2, gap=10;

    draw_button(sx,btn_y,btn_w,btn_h,"New Game",gs->btn_new_hover,true);
    draw_button(sx+btn_w+gap,btn_y,btn_w,btn_h,"Flip Board",gs->btn_flip_hover,false);

    int btn_y2=btn_y+btn_h+8;
    draw_button(sx,btn_y2,btn_w,btn_h,"Undo",gs->btn_undo_hover,false);
    draw_button(sx+btn_w+gap,btn_y2,btn_w,btn_h,"Resign",gs->btn_resign_hover,false);

    /* ── Turn / status indicator ── */
    int ind_y=630;
    if(gs->game_over) {
        draw_rounded_rect(sx,ind_y-26,SIDEBAR_W,24,6,0x333333ff);
        draw_text(g_font_sm,gs->result_str,sx+SIDEBAR_W/2,ind_y-14,
                  COL_TEXT_ACC,true,true);
    } else {
        bool check = gs->white_turn ? gs->white_in_check : gs->black_in_check;
        const char *status = check ? "CHECK!" : (gs->white_turn?"White to move":"Black to move");
        Uint32 status_col = check ? 0xFF4444ff : COL_TEXT_SEC;
        draw_rounded_rect(sx,ind_y-26,SIDEBAR_W,24,6,0x282828ff);
        /* Turn dot */
        Uint32 dot_col = gs->white_turn ? 0xF5F5F5ff : 0x222222ff;
        draw_circle_filled(sx+14,ind_y-14,6,dot_col);
        draw_circle_border(sx+14,ind_y-14,6,1,0x666666ff);
        draw_text(g_font_sm,status,sx+28,ind_y-14,status_col,false,true);
    }

    /* ── Logo / title ── */
    draw_text(g_font_lg,"VOID CHESS",sx+SIDEBAR_W/2,WINDOW_H/2-2,
              0x3A3A3Aff,true,true);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  DRAWING: BACKGROUND                                                */
/* ─────────────────────────────────────────────────────────────────── */
static void draw_background(void) {
    /* Dark gradient background */
    fill_rect(0,0,WINDOW_W,WINDOW_H,0x141414ff);
    /* Subtle texture lines */
    SDL_SetRenderDrawBlendMode(g_renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(g_renderer,255,255,255,3);
    for(int y=0;y<WINDOW_H;y+=4)
        SDL_RenderDrawLine(g_renderer,0,y,WINDOW_W,y);
    /* Board area panel */
    draw_rounded_rect(BOARD_MARGIN-12,BOARD_MARGIN-12,
                      BOARD_SIZE+24,BOARD_SIZE+24,4,0x1E1E1Eff);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  MAIN RENDER                                                        */
/* ─────────────────────────────────────────────────────────────────── */
static void render(void) {
    SDL_SetRenderDrawColor(g_renderer,20,20,20,255);
    SDL_RenderClear(g_renderer);

    draw_background();
    draw_board();
    draw_pieces();
    draw_sidebar();
    draw_promotion_picker();

    SDL_RenderPresent(g_renderer);
}

/* ─────────────────────────────────────────────────────────────────── */
/*  INPUT HANDLING                                                     */
/* ─────────────────────────────────────────────────────────────────── */
static bool is_btn(int mx,int my,int x,int y,int w,int h){
    return mx>=x&&mx<x+w&&my>=y&&my<y+h;
}

static void handle_square_click(int row, int col) {
    GameState *gs=&g_state;
    if(gs->game_over) return;

    /* Promotion picker */
    if(gs->promoting) return;

    Piece p=gs->board[row][col];
    bool own = gs->white_turn ? IS_WHITE(p) : IS_BLACK(p);

    if(gs->has_selection) {
        /* Try to find matching legal move */
        for(int i=0;i<gs->legal_count;i++) {
            Move *m=&gs->legal_moves[i];
            if(m->from_row==gs->selected_row&&m->from_col==gs->selected_col&&
               m->to_row==row&&m->to_col==col) {
                /* Check promotion */
                if(m->promotion!=EMPTY) {
                    /* Pick queen automatically for now */
                    /* (promotion UI would intercept here) */
                }
                apply_move(gs,m);
                return;
            }
        }
        /* Click own piece — re-select */
        if(own && !(row==gs->selected_row&&col==gs->selected_col)) {
            gs->selected_row=row; gs->selected_col=col;
            return;
        }
        /* Deselect */
        gs->has_selection=false;
        gs->selected_row=-1; gs->selected_col=-1;
    } else {
        if(own) {
            gs->has_selection=true;
            gs->selected_row=row; gs->selected_col=col;
        }
    }
}

static void handle_promotion_click(int mx,int my) {
    GameState *gs=&g_state;
    bool white=!gs->white_turn;
    Piece choices[4];
    if(white){choices[0]=W_QUEEN;choices[1]=W_ROOK;choices[2]=W_BISHOP;choices[3]=W_KNIGHT;}
    else     {choices[0]=B_QUEEN;choices[1]=B_ROOK;choices[2]=B_BISHOP;choices[3]=B_KNIGHT;}

    int px=board_x(gs->promo_col);
    int base_y=board_y(gs->promo_row);

    for(int i=0;i<4;i++) {
        int sy=base_y+i*SQUARE_SIZE;
        if(mx>=px&&mx<px+SQUARE_SIZE&&my>=sy&&my<sy+SQUARE_SIZE) {
            /* Apply promotion */
            for(int j=0;j<gs->legal_count;j++) {
                Move *m=&gs->legal_moves[j];
                if(m->from_row==gs->promo_from_row&&m->from_col==gs->promo_from_col&&
                   m->to_row==gs->promo_row&&m->to_col==gs->promo_col&&
                   m->promotion==choices[i]) {
                    gs->promoting=false;
                    apply_move(gs,m);
                    return;
                }
            }
            gs->promoting=false;
            return;
        }
    }
    gs->promoting=false;
}

static void update_button_hovers(int mx,int my) {
    GameState *gs=&g_state;
    int sx=SIDEBAR_X;
    int btn_y=640,btn_h=36,btn_w=(SIDEBAR_W-30)/2,gap=10;
    gs->btn_new_hover    = is_btn(mx,my,sx,btn_y,btn_w,btn_h);
    gs->btn_flip_hover   = is_btn(mx,my,sx+btn_w+gap,btn_y,btn_w,btn_h);
    int by2=btn_y+btn_h+8;
    gs->btn_undo_hover   = is_btn(mx,my,sx,by2,btn_w,btn_h);
    gs->btn_resign_hover = is_btn(mx,my,sx+btn_w+gap,by2,btn_w,btn_h);
}

static void handle_mouse_down(int mx,int my,int btn) {
    GameState *gs=&g_state;
    if(btn!=SDL_BUTTON_LEFT) return;

    /* Promotion picker */
    if(gs->promoting){ handle_promotion_click(mx,my); return; }

    /* Buttons */
    int sx=SIDEBAR_X;
    int btn_y=640,bh=36,bw=(SIDEBAR_W-30)/2,gap=10;
    if(is_btn(mx,my,sx,btn_y,bw,bh))          { new_game(gs); return; }
    if(is_btn(mx,my,sx+bw+gap,btn_y,bw,bh))   { gs->flipped=!gs->flipped; return; }
    int by2=btn_y+bh+8;
    if(is_btn(mx,my,sx,by2,bw,bh)) {
        /* Undo */
        if(gs->move_count>0&&!gs->game_over) {
            /* Simple undo: restart and replay */
            Move hist[512]; int cnt=gs->move_count-1;
            memcpy(hist,gs->move_history,sizeof(Move)*cnt);
            new_game(gs);
            for(int i=0;i<cnt;i++) {
                /* Find matching legal move */
                for(int j=0;j<gs->legal_count;j++) {
                    Move *m=&gs->legal_moves[j];
                    if(m->from_row==hist[i].from_row&&m->from_col==hist[i].from_col&&
                       m->to_row==hist[i].to_row&&m->to_col==hist[i].to_col&&
                       (hist[i].promotion==EMPTY||m->promotion==hist[i].promotion)) {
                        apply_move(gs,m); break;
                    }
                }
            }
            gs->anim.active=false;
        }
        return;
    }
    if(is_btn(mx,my,sx+bw+gap,by2,bw,bh)) {
        /* Resign */
        if(!gs->game_over) {
            gs->game_over=true;
            strcpy(gs->result_str, gs->white_turn ? "0-1  Black wins! White resigned." : "1-0  White wins! Black resigned.");
        }
        return;
    }

    /* Board click */
    int row,col;
    if(screen_to_board(mx,my,&row,&col)) {
        handle_square_click(row,col);
        /* Start drag */
        if(gs->has_selection&&row==gs->selected_row&&col==gs->selected_col) {
            gs->dragging=true;
            gs->drag_piece_row=row; gs->drag_piece_col=col;
            gs->drag_x=(float)mx; gs->drag_y=(float)my;
        }
    }
}

static void handle_mouse_up(int mx,int my,int btn) {
    GameState *gs=&g_state;
    if(btn!=SDL_BUTTON_LEFT||!gs->dragging) return;
    gs->dragging=false;
    int row,col;
    if(screen_to_board(mx,my,&row,&col)) {
        if(!(row==gs->drag_piece_row&&col==gs->drag_piece_col))
            handle_square_click(row,col);
    }
}

static void handle_mouse_motion(int mx,int my) {
    GameState *gs=&g_state;
    if(gs->dragging){ gs->drag_x=(float)mx; gs->drag_y=(float)my; }
    int row,col;
    if(screen_to_board(mx,my,&row,&col)){ gs->hover_row=row; gs->hover_col=col; }
    else { gs->hover_row=-1; gs->hover_col=-1; }
    update_button_hovers(mx,my);
}

static void handle_scroll(int dy) {
    g_state.move_list_scroll -= dy;
    if(g_state.move_list_scroll<0) g_state.move_list_scroll=0;
}

/* ─────────────────────────────────────────────────────────────────── */
/*  TIMER UPDATE                                                       */
/* ─────────────────────────────────────────────────────────────────── */
static void update_timers(void) {
    GameState *gs=&g_state;
    if(!gs->timed_game||gs->game_over) return;
    Uint32 now=SDL_GetTicks();
    float dt=(float)(now-gs->last_tick)/1000.0f;
    gs->last_tick=now;
    if(gs->white_turn) gs->w_time-=dt;
    else               gs->b_time-=dt;
    if(gs->w_time<=0){ gs->w_time=0; gs->game_over=true; strcpy(gs->result_str,"0-1  Black wins on time!"); }
    if(gs->b_time<=0){ gs->b_time=0; gs->game_over=true; strcpy(gs->result_str,"1-0  White wins on time!"); }
}

/* ─────────────────────────────────────────────────────────────────── */
/*  FONT LOADING  (embedded fallback using SDL_ttf built-in)          */
/* ─────────────────────────────────────────────────────────────────── */
static bool load_fonts(void) {
    /* Try common system fonts that support Unicode chess symbols */
    const char *candidates[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/freefont/FreeSans.ttf",
        "/System/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        NULL
    };
    const char *found = NULL;
    for(int i=0;candidates[i];i++) {
        FILE *f=fopen(candidates[i],"r");
        if(f){fclose(f);found=candidates[i];break;}
    }
    if(!found) { fprintf(stderr,"No suitable font found.\n"); return false; }

    g_font_lg = TTF_OpenFont(found, 22);
    g_font_md = TTF_OpenFont(found, 16);
    g_font_sm = TTF_OpenFont(found, 13);
    g_font_xs = TTF_OpenFont(found, 11);
    g_font_pieces = TTF_OpenFont(found, 40);
    return g_font_lg && g_font_md && g_font_sm && g_font_xs && g_font_pieces;
}

/* ─────────────────────────────────────────────────────────────────── */
/*  MAIN                                                               */
/* ─────────────────────────────────────────────────────────────────── */
int main(void) {
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
        fprintf(stderr,"SDL init failed: %s\n",SDL_GetError());
        return 1;
    }
    if(TTF_Init() < 0) {
        fprintf(stderr,"TTF init failed: %s\n",TTF_GetError());
        return 1;
    }

    /* Enable HiDPI */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window = SDL_CreateWindow(
        "Void Chess",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE
    );
    if(!g_window){ fprintf(stderr,"Window failed: %s\n",SDL_GetError()); return 1; }

    g_renderer = SDL_CreateRenderer(g_window, -1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!g_renderer){ fprintf(stderr,"Renderer failed: %s\n",SDL_GetError()); return 1; }

    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

    if(!load_fonts()) {
        fprintf(stderr,"Font loading failed.\n");
        /* Continue without fonts — pieces won't render well */
    }

    new_game(&g_state);
    fprintf(stdout,"Void Chess started.\n");
    fprintf(stdout,"Controls: Click/drag pieces | New Game | Flip | Undo | Resign\n");

    while(g_running) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT: g_running=false; break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym==SDLK_ESCAPE) g_running=false;
                    if(e.key.keysym.sym==SDLK_n) new_game(&g_state);
                    if(e.key.keysym.sym==SDLK_f) g_state.flipped=!g_state.flipped;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    handle_mouse_down(e.button.x,e.button.y,e.button.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    handle_mouse_up(e.button.x,e.button.y,e.button.button);
                    break;
                case SDL_MOUSEMOTION:
                    handle_mouse_motion(e.motion.x,e.motion.y);
                    break;
                case SDL_MOUSEWHEEL:
                    handle_scroll(e.wheel.y);
                    break;
            }
        }
        update_timers();
        render();
        SDL_Delay(8); /* ~120fps cap */
    }

    /* Cleanup */
    TTF_CloseFont(g_font_lg);
    TTF_CloseFont(g_font_md);
    TTF_CloseFont(g_font_sm);
    TTF_CloseFont(g_font_xs);
    TTF_CloseFont(g_font_pieces);
    TTF_Quit();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
    return 0;
}
