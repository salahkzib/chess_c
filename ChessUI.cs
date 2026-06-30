using System.Diagnostics;

namespace ChessC
{
    public partial class ChessUI : Form
    {
        public ChessUI()
        {
            InitializeComponent();
            UIInitializing();
            InitializingChess();
        }
        public Square[,] BoardSquares = new Square[8, 8];
        public Piece[] WPieces = new Piece[16];
        public Piece[] BPieces = new Piece[16];
        public Piece SelectedPiece = new Piece();
        public List<Square> SquareAbled = new List<Square>();
        public void UIInitializing()
        {
            this.BoardSquares = new Square[8, 8] {
                { SquareA0, SquareB0, SquareC0, SquareD0, SquareE0, SquareF0, SquareG0, SquareH0 },
                { SquareA1, SquareB1, SquareC1, SquareD1, SquareE1, SquareF1, SquareG1, SquareH1 },
                { SquareA2, SquareB2, SquareC2, SquareD2, SquareE2, SquareF2, SquareG2, SquareH2 },
                { SquareA3, SquareB3, SquareC3, SquareD3, SquareE3, SquareF3, SquareG3, SquareH3 },
                { SquareA4, SquareB4, SquareC4, SquareD4, SquareE4, SquareF4, SquareG4, SquareH4 },
                { SquareA5, SquareB5, SquareC5, SquareD5, SquareE5, SquareF5, SquareG5, SquareH5 },
                { SquareA6, SquareB6, SquareC6, SquareD6, SquareE6, SquareF6, SquareG6, SquareH6 },
                { SquareA7, SquareB7, SquareC7, SquareD7, SquareE7, SquareF7, SquareG7, SquareH7 }
            };
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    this.BoardSquares[i, j].IndexInBoard = new int[2] { i, j };
                    this.BoardSquares[i, j].OfficialColor = this.BoardSquares[i, j].BackColor;
                }
            }
            WPieces = new Piece[16];
            WPieces[0] = WRook1;
            WPieces[1] = WKnight1;
            WPieces[2] = WBishop1;
            WPieces[3] = WQueen;
            WPieces[4] = WKing;
            WPieces[5] = WBishop2;
            WPieces[6] = WKnight2;
            WPieces[7] = WRook2;
            WPieces[8] = WPawn1;
            WPieces[9] = WPawn2;
            WPieces[10] = WPawn3;
            WPieces[11] = WPawn4;
            WPieces[12] = WPawn5;
            WPieces[13] = WPawn6;
            WPieces[14] = WPawn7;
            WPieces[15] = WPawn8;

            BPieces = new Piece[16];
            BPieces[0] = BRook1;
            BPieces[1] = BKnight1;
            BPieces[2] = BBishop1;
            BPieces[3] = BQueen;
            BPieces[4] = BKing;
            BPieces[5] = BBishop2;
            BPieces[6] = BKnight2;
            BPieces[7] = BRook2;
            BPieces[8] = BPawn1;
            BPieces[9] = BPawn2;
            BPieces[10] = BPawn3;
            BPieces[11] = BPawn4;
            BPieces[12] = BPawn5;
            BPieces[13] = BPawn6;
            BPieces[14] = BPawn7;
            BPieces[15] = BPawn8;

            int index = 0;
            for (int i = 0; i < 2; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    WPieces[index].Position = new int[2] { i, j };
                    BPieces[index].Position = new int[2] { 7 - i, j };
                    index++;
                }
            }
        }

        private void ChessUI_Load(object sender, EventArgs e)
        {
        }
        private void Piece_Click(object sender, EventArgs e)
        {
            UnablingSquares();
            Piece UsedPiece = (Piece)sender;
            this.SelectedPiece = (Piece)sender;
            Tuple<int[,], int[], char> TuplePiece = MovesToPlayIn(UsedPiece.Position);
            int[,] AllMoves = TuplePiece.Item1;
            int[] TakenMoves = TuplePiece.Item2;
            char color = TuplePiece.Item3;
            int len = AllMoves.GetLength(0);
            int x, y;
            for (int i = 0; i < len; i++)
            {
                x = AllMoves[i, 0];
                y = AllMoves[i, 1];
                this.BoardSquares[x, y].Enabled = true;
                this.BoardSquares[x, y].BackColor = Color.Green;
                this.SquareAbled.Add(this.BoardSquares[x, y]);
            }
            len = TakenMoves.Length;
            int index;
            if(color == 'b')
            {
                for (int i = 0; i < len; i++)
                {
                    index = TakenMoves[i];
                    this.WPieces[index].Click -= Piece_Click;
                    this.WPieces[index].Click += Piece_ToBe_Taked;
                    this.WPieces[index].BackColor = Color.Green;
                }
            }
            else if(color == 'w')
            {
                for (int i = 0; i < len; i++)
                {
                    index = TakenMoves[i];
                    this.BPieces[index].Click -= Piece_Click;
                    this.BPieces[index].Click += Piece_ToBe_Taked;
                    this.BPieces[index].BackColor = Color.Green;
                }
            }
        }

        public void Square_Click(object sender, EventArgs e)
        {
            Square SquareSelected = (Square)sender;
            if (SquareSelected != null)
            {
                SelectedPiece.Location = SquareSelected.Location;
                SelectedPiece.BackColor = SquareSelected.OfficialColor;
                CheckingMovePlayed(SelectedPiece.Position, SquareSelected.IndexInBoard, false);
                SelectedPiece.Position = SquareSelected.IndexInBoard;
                SelectedPiece.BringToFront();
                UnablingSquares();
            }
        }

        public void Piece_ToBe_Taked(object sender, EventArgs e)
        {
            Piece piece = (Piece)sender;
            Square SquareSelected = this.BoardSquares[piece.Position[0], piece.Position[1]];
            piece.Hide();
            SelectedPiece.Location = SquareSelected.Location;
            SelectedPiece.BackColor = SquareSelected.OfficialColor;
            CheckingMovePlayed(SelectedPiece.Position, SquareSelected.IndexInBoard, true);
            SelectedPiece.Position = SquareSelected.IndexInBoard;
            SelectedPiece.BringToFront();
            UnablingSquares();
        }

        private void UnablingSquares()
        {
            for (int i = 0; i < this.SquareAbled.Count; i++)
            {
                this.SquareAbled[i].BackColor = this.SquareAbled[i].OfficialColor;
                this.SquareAbled[i].Enabled = false;
            }
            this.SquareAbled.Clear();
        }
    }
}
