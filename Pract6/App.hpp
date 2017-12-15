#ifndef APP_HPP
#define APP_HPP

//Singleton

class App {
    public:
    static App& instance();    
    void setup(int argc, char **argv);
    
    void run();
    
    
    private:
    App();
    static void iddleFunc();
    static void displayFunc();
    static void reshapeFunc(int w, int h);
    
    static App* theInstance;
};

#endif
