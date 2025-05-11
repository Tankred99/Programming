import javax.swing.JFrame;
import javax.swing.JLabel;

public class testJava {

    public static void main(String[] args) {
        JFrame frame = new JFrame("Title");
        JLabel label = new JLabel("hello world");
        frame.add(label);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(200, 100);
        frame.setVisible(true);
    }
}
