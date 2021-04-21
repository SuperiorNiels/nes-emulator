#ifndef NES_VIEW_H
#define NES_VIEW_H

class View {
    // View base interface
    virtual bool render() const = 0;
}

#endif // NES_VIEW_H