using System.Diagnostics;

namespace ChessC
{
    public partial class ChessUI : Form
    {
        public Square[,] BoardSquares;
        public Piece SelectedPiece;
        public List<Square> SquareAbled;
        public ChessUI()
        {
            InitializeComponent();
            Initializing();

        }
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
                    BoardSquares[i, j].IndexInBoard = new int[2] { i, j };
                    BoardSquares[i, j].OficialColor = BoardSquares[i, j].BackColor;
                }
            }
        }
        private void ChessUI_Load(object sender, EventArgs e)
        {

        }

        public int[,] MovesToPlayIn(int[] PiecePosition)
        {
            int[,] Moves = { };
            return Moves;
        }
        public void CheckMovePlayed(int[] WhereItWas, int[] WhereItIs)
        {

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
                    CheckMovePlayed(SelectedPiece.Position, SquareSelected.IndexInBoard);
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
