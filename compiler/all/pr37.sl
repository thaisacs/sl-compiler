// Incompatible array types

void Example()
  types
     Vector = integer[100];
     Matrix = Vector[10];

  vars
    M: Matrix;
    V: Vector;
    
{
  M = V;
}
