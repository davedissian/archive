#ifndef WAYPOINT_H
#define WAYPOINT_H

class Waypoint
{
public:
    struct Node
    {
        Node(const Vec2i& p) : mPosition(p) {}
        Vec2i mPosition;
    };

    typedef vector<Node>::iterator NodeIterator;

    Waypoint();
    ~Waypoint();

    void AddPosition(const Vec2i& p);
    void DebugDump();

    // Allow iteration
    NodeIterator begin();
    NodeIterator end();

private:
    vector<Node> mNodes;
};;

#endif /* WAYPOINT_H */
