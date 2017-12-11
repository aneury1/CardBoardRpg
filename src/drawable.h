#ifndef DRAWABLE_H
#define DRAWABLE_H


class Drawable
{
    public:
        virtual ~Drawable(){}
        virtual int GetDownY()=0;
        virtual void Draw()=0;
};

#endif // DRAWABLE_H
