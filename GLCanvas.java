import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import java.nio.file.Path;
import java.nio.file.Paths;

public class GLCanvas extends Canvas {
    static {
        Path libPath = Paths.get(
                System.mapLibraryName("litegfxawt")
        ).toAbsolutePath();

        System.load(libPath.toString());
    }

    private long context = 0L;

    private native long createContext();
    private native void paint(long context);

    public void paint(Graphics g) {
        paint(context);
    }

    public void update(Graphics g) {
        paint(context);
    }

    public void addNotify()
    {
        super.addNotify();
        context = createContext();
    }

    public static void main(String[] args) throws InterruptedException {
        JFrame f = new JFrame("Java AWT GLX");
        f.setSize(800, 600);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        GLCanvas canvas = new GLCanvas();
        f.add(canvas);
        f.setVisible(true);

        while (f.isVisible())
            canvas.repaint();
    }
}
