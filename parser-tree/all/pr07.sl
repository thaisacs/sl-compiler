// Multiple variables and assignments

void Example()
  vars x, y, z: integer;
       p, q: boolean;
{
  y[10][10] = 100;
  x = y;
  q = true;
  p = q;
  write(x, y);
}
