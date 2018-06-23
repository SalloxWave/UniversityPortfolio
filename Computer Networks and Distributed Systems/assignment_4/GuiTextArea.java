import javax.swing.*;    
import java.awt.*;    

public class GuiTextArea {
    
    JTextArea myArea;
	JFrame frame;
	JScrollPane scrollPane;
	private static int numOfWindowsOnX = 0;
	private static int numOfWindowsOnY = 0;

    //--------------------
    GuiTextArea(String title) {
		
		//Create and set up the window
		frame = new JFrame(title);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		myArea = new JTextArea(20, 61);
		myArea.setEditable(false);
		myArea.setFont(new Font("monospaced", Font.PLAIN, 12));
		scrollPane = 
			new JScrollPane(myArea,
					JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
					JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);

		
		frame.getContentPane().add(scrollPane);
		
		//Display the window.
		frame.pack();

		//WARNING: UGLY SOLUTION!!!
		//Set location to based on number of windows opened in program
		if (frame.getWidth() * (numOfWindowsOnX+1) > Toolkit.getDefaultToolkit().getScreenSize().getWidth())
		{
			numOfWindowsOnY++;
			numOfWindowsOnX = 0;
		}
		int x = frame.getWidth() * numOfWindowsOnX;
		int y = frame.getHeight() * numOfWindowsOnY;	
		frame.setLocation(x, y);
		numOfWindowsOnX++;
		
		frame.setVisible(true);
    }

    //--------------------
    public void print(String s)   { 
	myArea.append(s); 
        myArea.setCaretPosition(myArea.getDocument().getLength());
    }
    public void println(String s) { print(s+"\n"); }
    public void println()         { print("\n"); }

	//New methods
	public void scrollToTop() 
	{ 
		myArea.setCaretPosition(0);
		frame.revalidate();
		frame.repaint();
	}

}
