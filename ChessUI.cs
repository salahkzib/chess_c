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
            WPieces = new Piece[16] { WRook1, WKnight1, WBishop1, WQueen, WKing, WBishop2, WKnight2, WRook2, WPawn1, WPawn2, WPawn3, WPawn4, WPawn5, WPawn6, WPawn7, WPawn8 };
            BPieces = new Piece[16] { BRook1, BKnight1, BBishop1, BQueen, BKing, BBishop2, BKnight2, BRook2, BPawn1, BPawn2, BPawn3, BPawn4, BPawn5, BPawn6, BPawn7, BPawn8 };
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
            int[,] SquaresToMoveOn = MovesToPlayIn(UsedPiece.Position);
            int len = SquaresToMoveOn.GetLength(0);
            int x, y;
            for (int i = 0; i < len; i++)
            {
                x = SquaresToMoveOn[i, 0];
                y = SquaresToMoveOn[i, 1];
                this.BoardSquares[x, y].Enabled = true;
                this.BoardSquares[x, y].BackColor = Color.Green;
                this.SquareAbled.Add(this.BoardSquares[x, y]);
            }
        }

        public void Square_Click(object sender, EventArgs e)
        {
            Square SquareSelected = (Square)sender;
            if (SquareSelected != null)
            {
                SelectedPiece.Location = SquareSelected.Location;
                SelectedPiece.BackColor = SquareSelected.OfficialColor;
                CheckingMovePlayed(SelectedPiece.Position, SquareSelected.IndexInBoard);
                SelectedPiece.Position = SquareSelected.IndexInBoard;
                SelectedPiece.BringToFront();
                UnablingSquares();
            }
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
