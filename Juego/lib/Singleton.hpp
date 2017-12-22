#ifndef SINGLETON_HPP
#define	SINGLETON_HPP


template <class T>
/**
 * @brief Clase base para otras clases singleton.
 * Para usar esta clase: 1. hereda de la clase Singleton y pasa como argumento
 * la clase base. 2. La clase base debe declararse como amiga (friend class)
 * de la clase Singleton. 3. Haz que el constructor de la clase hija sea privado.
 */
class Singleton {

    private:
    protected:        
    Singleton() {}
    public:
    static T& instance() {
        static T t;
        return t;
    }


};

#endif
