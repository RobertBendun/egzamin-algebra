# inverse_modulo.cc
Compile program with:
```
g++ inverse_modulo.cc -o inverse_modulo -std=c++17
```
And run:
```
./inverse_modulo [number] [base]
```

# polynomials_modulo.cc
Compile program with:
```
g++ polynomials_modulo.cc -o polynomials_modulo -std=c++17
```
And run:
```
./polynomials_modulo
```
Provide data space separated:
```
Base: 5
1: 1 4 0 3 0 2
2: 2 0 1 4
Op (+, -, *, /): /
[ 1 4 0 3 0 2 ] = [ 2 0 1 4 ] * [ 3 2 1 ] + [ 4 1 3 ]
```
Example above calculates `(x^5 + 4^4 + 3x^2 + 2) / (2x^3 + x + 4) [mod 5]`
