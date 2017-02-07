package exa.modeller.threed;

import exa.modeller.math.*;
import exa.modeller.util.*;
import java.awt.Color;

public final class Cube extends Polygonal {
    public Cube() {
        super();
        addVertex(new Vertex(new Vector3(0, 0, 0)));
        addVertex(new Vertex(new Vector3(1, 0, 0)));
        addVertex(new Vertex(new Vector3(1, 1, 0)));
        addVertex(new Vertex(new Vector3(0, 1, 0)));
        addVertex(new Vertex(new Vector3(0, 0, 1)));
        addVertex(new Vertex(new Vector3(1, 0, 1)));
        addVertex(new Vertex(new Vector3(1, 1, 1)));
        addVertex(new Vertex(new Vector3(0, 1, 1)));
        addSurface(new PolygonMesh());
	getSurface(0).attr=new Attribute(Color.yellow, 0.2, 0.6, 0.3, 3);
        getSurface(0).addPolygon(new Polygon3d(this, 0, 1, 2, 3));
        getSurface(0).addPolygon(new Polygon3d(this, 7, 6, 5, 4));
        getSurface(0).addPolygon(new Polygon3d(this, 4, 5, 1, 0));
        getSurface(0).addPolygon(new Polygon3d(this, 5, 6, 2, 1));
        getSurface(0).addPolygon(new Polygon3d(this, 6, 7, 3, 2));
        getSurface(0).addPolygon(new Polygon3d(this, 7, 4, 0, 3));
	calcN();
    }
}
