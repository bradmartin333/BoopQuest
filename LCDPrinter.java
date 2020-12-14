import java.util.*;
import java.io.*;
public class LCDPrinter {
    public static void main(String[] args) {
      boolean forever = true;
      boolean top = false;
      boolean bot = false;
      boolean url = false;
      int line = 0;
      int urlIndent = 0;
      System.out.println();
      System.out.println(" 16x2 LCD Print Formatter      (Type \"exit\" to leave)");
      System.out.println("------------------------------------------------------");
      do{
	//Initialize scanner and parameters
	Scanner keyboard = new Scanner(System.in);
	System.out.print("Enter Phrase: ");
	String Entry = keyboard.nextLine();
	if (Entry.equals("exit")) {
	    forever = false;
	    break;
	}
	if (Entry.equals("top")) {
	    line = 0;
	    System.out.print("Top Mode Enabled");
	    top = true;
	    bot = false;
	    url = false;
	}
	if (Entry.equals("bot")) {
	    line = 1;
	    System.out.print("Bottom Mode Enabled");
	    bot = true;
	    top = false;
	    url = false;
	}
	if (Entry.equals("url")) {
	    line = 0;
	    System.out.print("TinyUrl Mode Enabled");
	    bot = false;
	    top = false;
	    url = true;
	}
	if (top == true) {
	    line = 0;
	}
	else if (bot == true) {
	    line = 1;
	}
	else if (top == false && bot == false && url == false) {
	    line = 0;
	}
	System.out.println();
	System.out.printf("Length = %d", Entry.length());
	System.out.println();
	boolean wrapper = true;
	int wrap = 16;
	if (Entry.length() > 16) {
	    wrap = (Entry.lastIndexOf(' ', 16));
	}
	
	//Convert Entry to ASCII String
	StringBuilder builder = new StringBuilder();
	for (int x=0; x<Entry.length(); x++) {
	    char letter = Entry.charAt(x);
	    int ASCII = (int)letter;
	    builder.append(Integer.toString(ASCII));
	    builder.append(" ");
	}
	String Phrase = builder.toString();

	//Tokenize and format ASCII String for LCD
	int n = 0;
	int y = 0;
	StringTokenizer Tok = new StringTokenizer(Phrase);
	if (url == true) {
	    urlIndent = 4;
	    System.out.print("lcd.setCursor(2,0);b[0]=116;b[1]=105;b[2]=110;b[3]=121;b[4]=117;b[5]=114;b[6]=108;b[7]=46;b[8]=99;b[9]=111;b[10]=109;b[11]=47;b[12]=0;lcd.print(x.y(b));");
	    line = 1;
	}
	else {
	    System.out.println();
	    urlIndent = 0;
	}
	System.out.printf("lcd.setCursor(%d,%d);", urlIndent, line);
	while (Tok.hasMoreElements()) {
	    System.out.printf("b[%d]=%s;", n, Tok.nextElement());
	    n++;
	    y++;
	    if (n > wrap && wrapper==true) {
		line++;
		if (n < 16) {
		    System.out.printf("b[%d]=0;", n); //Adds end character if necessary
		}
		System.out.print("lcd.print(x.y(b));");
		System.out.printf("lcd.setCursor(0,%d);", line);
		n = 0;
		y += (16-wrap);
		wrapper = false;
	    }
	}
	if (n < 16) {
	    System.out.printf("b[%d]=0;", n); //Adds end character if necessary
	}
	System.out.print("lcd.print(x.y(b));");
	System.out.println();
	System.out.println();
	
	//Check for validity and give info
	if (wrapper == false) {
	    System.out.println("----    TWO LINES    ----");
	}
	if (y > 32) {
	    System.out.printf("XXXX    TOO LONG BY %d    XXXX", (y-32));
	    System.out.println();
	}

	//End formatting
	System.out.println("------------------------------------------------------");
      }
      while (forever == true);
    }
}    
