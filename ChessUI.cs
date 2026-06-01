using System.Diagnostics;

namespace ChessC
{
    public partial class ChessUI : Form
    {
        public ChessUI()
        {
            InitializeComponent();
            Initializing();
        }
        public Square[,] BoardSquares = new Square[8, 8];
        public Piece[] WPieces = new Piece[16];
        public Piece[] BPieces = new Piece[16];
        public Piece SelectedPiece = new Piece();
        public List<Square> SquareAbled = new List<Square>();
        public void Initializing()
        {
            this.BoardSquares = new Square[8,8] { 
                {SquareA0, SquareA1, SquareA2, SquareA3, SquareA4, SquareA5, SquareA6, SquareA7 },
                {SquareB0, SquareB1, SquareB2, SquareB3, SquareB4, SquareB5, SquareB6, SquareB7 },
                {SquareC0, SquareC1, SquareC2, SquareC3, SquareC4, SquareC5, SquareC6, SquareC7 },
                {SquareD0, SquareD1, SquareD2, SquareD3, SquareD4, SquareD5, SquareD6, SquareD7 },
                {SquareE0, SquareE1, SquareE2, SquareE3, SquareE4, SquareE5, SquareE6, SquareE7 },
                {SquareF0, SquareF1, SquareF2, SquareF3, SquareF4, SquareF5, SquareF6, SquareF7 },
                {SquareG0, SquareG1, SquareG2, SquareG3, SquareG4, SquareG5, SquareG6, SquareG7 },
                {SquareH0, SquareH1, SquareH2, SquareH3, SquareH4, SquareH5, SquareH6, SquareH7 }
            };
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    this.BoardSquares[i, j].IndexInBoard = new int[2] { i, j };
                    this.BoardSquares[i, j].OficialColor = this.BoardSquares[i, j].BackColor;
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
            Initializing();
        }
        private void Piece_Click(object sender, EventArgs e)
        {
            try
            {
                Piece UsedPiece = (Piece)sender;
                this.SelectedPiece = (Piece)sender;
                int[,] SquaresToMoveOn = MovesToPlayIn(UsedPiece.Position);
                int x, y;
                for (int i = 0; i < SquaresToMoveOn.Length; i++)
                {
                    x = SquaresToMoveOn[i, 0];
                    y = SquaresToMoveOn[i, 1];
                    this.BoardSquares[x, y].Enabled = true;
                    this.BoardSquares[x, y].BackColor = Color.Green;
                    this.SquareAbled.Add(this.BoardSquares[x, y]);
                }
            }
            catch{ }
        }

        private void Square_Click(object sender, EventArgs e)
        {
            try
            {
                Square SquareSelected = (Square)sender;
                if (SquareSelected != null)
                {
                    SelectedPiece.Location = SquareSelected.Location;
                    SelectedPiece.BackColor = SquareSelected.OficialColor;
                    CheckingMovePlayed(SelectedPiece.Position, SquareSelected.IndexInBoard);
                    SelectedPiece.Position = SquareSelected.IndexInBoard;
                    SelectedPiece.BringToFront();
                    for(int i = 0; i < this.SquareAbled.Count; i++)
                    {
                        this.SquareAbled[i].BackColor = this.SquareAbled[i].OficialColor;
                        this.SquareAbled[i].Enabled = false;
                    }
                    this.SquareAbled.Clear();
                }
            }
            catch { }
        }
    }
}
