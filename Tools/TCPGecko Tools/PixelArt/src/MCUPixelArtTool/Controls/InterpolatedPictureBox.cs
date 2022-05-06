using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace PixelArtTool.Controls {
	public class InterpolatedPictureBox : PictureBox
	{
		public InterpolationMode InterpolationMode { get; set; }

		protected override void OnPaint(PaintEventArgs paintEventArgs)
		{
			paintEventArgs.Graphics.InterpolationMode = this.InterpolationMode;
			base.OnPaint(paintEventArgs);
		}
	}
}
