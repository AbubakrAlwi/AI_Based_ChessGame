#include "board.h"
#include <iostream>

Board::Board(){ reset(); }

void Board::reset(){
    // empty
    for(int r=0;r<8;r++) for(int c=0;c<8;c++) b[r][c]=PieceOnSquare();
    // set up white (bottom) and black (top)
    // Black major pieces (rank 8 -> r=0)
    place_piece(0,0,ROOK,BLACK); place_piece(0,1,KNIGHT,BLACK); place_piece(0,2,BISHOP,BLACK); place_piece(0,3,QUEEN,BLACK);
    place_piece(0,4,KING,BLACK); place_piece(0,5,BISHOP,BLACK); place_piece(0,6,KNIGHT,BLACK); place_piece(0,7,ROOK,BLACK);
    // black pawns r=1
    for(int c=0;c<8;c++) place_piece(1,c,PAWN,BLACK);
    // white pawns r=6
    for(int c=0;c<8;c++) place_piece(6,c,PAWN,WHITE);
    // white major pieces r=7
    place_piece(7,0,ROOK,WHITE); place_piece(7,1,KNIGHT,WHITE); place_piece(7,2,BISHOP,WHITE); place_piece(7,3,QUEEN,WHITE);
    place_piece(7,4,KING,WHITE); place_piece(7,5,BISHOP,WHITE); place_piece(7,6,KNIGHT,WHITE); place_piece(7,7,ROOK,WHITE);

    side_to_move = WHITE;
    history.clear();
}

void Board::show() const{
    std::cout << "  +-----------------+\n";
    for(int r=0;r<8;r++){
        std::cout << (8-r) << " | ";
        for(int c=0;c<8;c++){
            auto pcs = b[r][c];
            char ch = '.';
            if(pcs.p!=EMPTY){
                switch(pcs.p){
                    case PAWN: ch='p'; break;
                    case KNIGHT: ch='n'; break;
                    case BISHOP: ch='b'; break;
                    case ROOK: ch='r'; break;
                    case QUEEN: ch='q'; break;
                    case KING: ch='k'; break;
                    default: ch='?';
                }
                if(pcs.c==WHITE) ch = toupper(ch);
            }
            std::cout << ch << ' ';
        }
        std::cout << "|\n";
    }
    std::cout << "  +-----------------+\n";
    std::cout << "    a b c d e f g h\n";
    std::cout << "Side to move: " << (side_to_move==WHITE?"White":"Black") << "\n";
}

bool Board::move_from_uci(const std::string &m){
    if(m.size()<4) return false;
    int fc = m[0]-'a';
    int fr = 7 - (m[1]-'1');
    int tc = m[2]-'a';
    int tr = 7 - (m[3]-'1');
    if(!in_bounds(fr,fc) || !in_bounds(tr,tc)) return false;
    Piece promo = EMPTY;
    if(m.size()>=5){ char p=m[4]; if(p=='q' || p=='Q') promo=QUEEN; }
    Move mv(fr,fc,tr,tc,promo);
    // check it is among legal moves
    auto moves = generate_moves(side_to_move);
    for(auto &mm: moves){
        if(mm.from_r==mv.from_r && mm.from_c==mv.from_c && mm.to_r==mv.to_r && mm.to_c==mv.to_c){
            if(mm.promotion!=EMPTY) mv.promotion = mm.promotion;
            make_move(mv);
            return true;
        }
    }
    return false;
}

std::vector<Move> Board::generate_moves(Color side) const{
    std::vector<Move> out;
    for(int r=0;r<8;r++) for(int c=0;c<8;c++){
        auto pcs = b[r][c];
        if(pcs.p==EMPTY || pcs.c!=side) continue;
        switch(pcs.p){
            case PAWN: gen_pawn_moves(r,c,side,out); break;
            case KNIGHT: gen_knight_moves(r,c,side,out); break;
            case BISHOP: {
                std::vector<std::pair<int,int>> dirs={{1,1},{1,-1},{-1,1},{-1,-1}}; gen_sliding_moves(r,c,side,dirs,out); break;
            }
            case ROOK: {
                std::vector<std::pair<int,int>> dirs={{1,0},{-1,0},{0,1},{0,-1}}; gen_sliding_moves(r,c,side,dirs,out); break;
            }
            case QUEEN: {
                std::vector<std::pair<int,int>> dirs={{1,1},{1,-1},{-1,1},{-1,-1},{1,0},{-1,0},{0,1},{0,-1}}; gen_sliding_moves(r,c,side,dirs,out); break;
            }
            case KING: gen_king_moves(r,c,side,out); break;
            default: break;
        }
    }
    return out;
}

void Board::gen_pawn_moves(int r,int c, Color col, std::vector<Move>&out) const{
    int dir = (col==WHITE? -1 : +1);
    int sr = r + dir;
    if(in_bounds(sr,c) && b[sr][c].p==EMPTY){
        // forward
        if((col==WHITE && sr==0) || (col==BLACK && sr==7)){
            out.emplace_back(r,c,sr,c,QUEEN); // promote to queen
        } else out.emplace_back(r,c,sr,c);
        // double
        int start_row = (col==WHITE?6:1);
        int dbl = r + 2*dir;
        if(r==start_row && in_bounds(dbl,c) && b[dbl][c].p==EMPTY) out.emplace_back(r,c,dbl,c);
    }
    // captures
    for(int dc : {-1,1}){
        int tc = c + dc;
        if(in_bounds(sr,tc) && b[sr][tc].p!=EMPTY && b[sr][tc].c!=col){
            if((col==WHITE && sr==0) || (col==BLACK && sr==7)){
                out.emplace_back(r,c,sr,tc,QUEEN);
            } else out.emplace_back(r,c,sr,tc);
        }
    }
}

void Board::gen_knight_moves(int r,int c, Color col, std::vector<Move>&out) const{
    const int drs[8] = {2,2,-2,-2,1,1,-1,-1};
    const int dcs[8] = {1,-1,1,-1,2,-2,2,-2};
    for(int i=0;i<8;i++){
        int nr=r+drs[i], nc=c+dcs[i];
        if(!in_bounds(nr,nc)) continue;
        if(b[nr][nc].p==EMPTY || b[nr][nc].c!=col) out.emplace_back(r,c,nr,nc);
    }
}

void Board::gen_sliding_moves(int r,int c, Color col, const std::vector<std::pair<int,int>>&dirs, std::vector<Move>&out) const{
    for(auto d:dirs){
        int nr=r+d.first, nc=c+d.second;
        while(in_bounds(nr,nc)){
            if(b[nr][nc].p==EMPTY){ out.emplace_back(r,c,nr,nc); }
            else{
                if(b[nr][nc].c!=col) out.emplace_back(r,c,nr,nc);
                break;
            }
            nr+=d.first; nc+=d.second;
        }
    }
}

void Board::gen_king_moves(int r,int c, Color col, std::vector<Move>&out) const{
    for(int dr=-1;dr<=1;dr++) for(int dc=-1;dc<=1;dc++){
        if(dr==0 && dc==0) continue;
        int nr=r+dr, nc=c+dc;
        if(!in_bounds(nr,nc)) continue;
        if(b[nr][nc].p==EMPTY || b[nr][nc].c!=col) out.emplace_back(r,c,nr,nc);
    }
}

bool Board::make_move(const Move &m){
    // naive make move, no legality checks except bounds
    PieceOnSquare captured = b[m.to_r][m.to_c];
    history.push_back({m, captured});
    PieceOnSquare mover = b[m.from_r][m.from_c];
    b[m.to_r][m.to_c] = mover;
    b[m.from_r][m.from_c] = PieceOnSquare();
    if(m.promotion!=EMPTY){
        b[m.to_r][m.to_c].p = m.promotion;
    }
    side_to_move = (side_to_move==WHITE?BLACK:WHITE);
    return true;
}

void Board::unmake_move(){
    if(history.empty()) return;
    auto u = history.back(); history.pop_back();
    auto m = u.m;
    b[m.from_r][m.from_c] = b[m.to_r][m.to_c];
    b[m.to_r][m.to_c] = u.captured;
    side_to_move = (side_to_move==WHITE?BLACK:WHITE);
}

int Board::evaluate(Color side) const{
    // simple material values
    static int val[7] = {0,100,320,330,500,900,20000};
    int score = 0;
    for(int r=0;r<8;r++) for(int c=0;c<8;c++){
        auto pcs = b[r][c];
        if(pcs.p==EMPTY) continue;
        int v = val[pcs.p];
        score += (pcs.c==WHITE? v : -v);
    }
    return score * (side==WHITE?1:-1);
}
