J=print
E=len
C=range
import sys,time as A,random as U,tracemalloc as K,math as L
B=int(sys.argv[1])
def N(n):return[[0]*n for A in C(n)]
def D(A,B):return[[A[D][C]+B[D][C]for C in C(E(A))]for D in C(E(A))]
def G(A,B):return[[A[D][C]-B[D][C]for C in C(E(A))]for D in C(E(A))]
def M(A,B):
	D=E(A);F=N(D)
	for G in C(D):
		for H in C(D):
			for I in C(D):F[G][I]+=A[G][H]*B[H][I]
	return F
def O(M):A=E(M)//2;return[B[:A]for B in M[:A]],[B[A:]for B in M[:A]],[B[:A]for B in M[A:]],[B[A:]for B in M[A:]]
def V(C11,C12,C21,C22):return[C11[A]+C12[A]for A in C(E(C11))]+[C21[A]+C22[A]for A in C(E(C21))]
def F(A,B):
	W=E(A)
	if W<=64:return M(A,B)
	C,K,L,H=O(A);I,N,P,J=O(B);Q=F(D(C,H),D(I,J));R=F(D(L,H),I);S=F(C,G(N,J));T=F(H,G(P,I));U=F(D(C,K),J);X=F(G(L,C),D(I,N));Y=F(G(K,H),D(P,J));Z=D(G(D(Q,T),U),Y);a=D(S,U);b=D(R,T);c=D(G(D(Q,S),R),X);return V(Z,a,b,c)
def P(M,n):
	A=N(n)
	for B in C(E(M)):
		for D in C(E(M)):A[B][D]=M[B][D]
	return A
def Q(n):return[[U.random()for A in C(n)]for A in C(n)]
R=1<<L.ceil(L.log2(B))
S,T=Q(B),Q(B)
W,X=P(S,R),P(T,R)
K.start()
H=A.perf_counter()
M(S,T)
I=A.perf_counter()
J(f"Naive Algorithm Time: {I-H:.6f}")
H=A.perf_counter()
F(W,X)
I=A.perf_counter()
J(f"Strassen Algorithm Time: {I-H:.6f}")
Z,Y=K.get_traced_memory()
J(f"Peak RAM (MB): {Y/1024/1024:.2f}")