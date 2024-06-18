;;; Treap data structure (декартово дерево)
        
        %include 'io.inc'
        extern scanf
        extern printf
        extern malloc
        extern rand
        extern free
        extern srand
        extern time

;;; struct node {int x, int y, int v, node *l, node *r} --- 20 bytes
;;;              0      4      8      12       16
        
        section .data
        root dd 0               ; node *root = NULL

        section .bss
        buf resb 10             ; input buffer
        found resd 1            ; flag
        x resd 1
        y resd 1


        section .rodata
        fmt1 db "%d", 0
        fmt2 db "%d %d", 10, 0
        fmt3 db "%s", 0
        fmt4 db "%d%d", 0
        



        global CMAIN
        section .text
CMAIN:
        sub esp, 44

        mov dword[esp], 0       ;
        call time               ;
        mov [esp], eax          ;
        call srand              ; srand(time(NULL))

.loop:
        mov dword[esp], fmt3    ;
        mov dword[esp+4], buf   ;
        call scanf              ; scanf("%s", buf)

        cmp byte[buf], 'F'      ; if F then break
        je .end                 ;
        cmp byte[buf], 'A'      ; if A then
        jne .l1                 ;
        mov dword[esp], fmt4    ;
        mov dword[esp+4], x     ;
        mov dword[esp+8], y     ;
        call scanf              ; scanf("%d%d", &x, &y)
        
        mov eax, [x]            ;
        mov [esp], eax          ;
        mov eax, [y]            ;
        mov [esp+4], eax        ;
        call insert             ; insert(x, y)
        jmp .tail
.l1:
	cmp byte[buf], 'D'      ; if D then
        jne .l2                 ;
        mov dword[esp], fmt1    ;
        mov dword[esp+4], x     ;
        call scanf              ; scanf("%d", &x)
        
        mov eax, [x]            ;
        mov [esp], eax          ;
        call erase              ; erase(x)
        jmp .tail
.l2:                            ; if S then 
        mov dword[found], 0     ;
        mov dword[esp], fmt1    ;
        mov dword[esp+4], x     ;
        call scanf              ; scanf("%d", &x)
        mov eax, [x]            ;
        mov [esp], eax          ;
        call search             ; res = search(x)
        cmp dword[found], 0     ; if (found) then
        je .tail
        mov [esp+8], eax        ;
        mov eax, [x]            ;
        mov [esp+4], eax        ;
        mov dword[esp], fmt2    ;
        call printf             ; printf("%d %d\n", x, res)
.tail:
	jmp .loop
.end:
        mov eax, [root]         ;
        mov [esp], eax          ;
        call clean              ; clean(root) -- clean used memory

        add esp, 44             ;
        xor eax, eax            ;
        ret                     ; return 0


;;; ======== node* make_node(int key, int val) ========
make_node:
        push ebp
        mov ebp, esp
        sub esp, 8
        mov dword[esp], 20      ;
        call malloc             ; 
        mov ebx, eax            ; node *p = malloc(20)
        
        mov ecx, [ebp+8]        ;
        mov [ebx], ecx          ; p->x = key
        
        mov ecx, [ebp+12]       ;
        mov [ebx+8], ecx        ; p->v = val

        call rand               ;
        mov [ebx+4], eax        ; p->y = rand()

        mov dword[ebx+12], 0    ; p->l = NULL
        mov dword[ebx+16], 0    ; p->r = NULL

        mov eax, ebx

        mov esp, ebp
        pop ebp
        ret

;;; ========= node *merge(node *a, node *b) ========
merge:
        push ebp
        mov ebp, esp
        sub esp, 8

        cmp dword[ebp+8], 0     ; if (!a) then
        jne .l1                 ;
        mov eax, [ebp+12]       ; return b
        jmp .end                ;
.l1:
        cmp dword[ebp+12], 0    ; if (!b) then
        jne .l2                 ;
        mov eax, [ebp+8]        ; return a
        jmp .end                ;
.l2:
	mov eax, [ebp+8]        ;
        mov eax, [eax+4]        ; eax = a->y

        mov ecx, [ebp+12]       ;
        mov ecx, [ecx+4]        ; ecx = b->y

        cmp eax, ecx            ; if a->y <= b->y
        jg .l3                  ; then
        mov eax, [ebp+12]       ;
        mov eax, [eax+12]       ; b->l
        mov [esp+4], eax        ;
        mov eax, [ebp+8]        ; 
        mov [esp], eax          ; a
        call merge              ;
        mov ecx, [ebp+12]       ;
        mov [ecx+12], eax       ; b->l = merge(a, b->l)
        mov eax, [ebp+12]       ;
        jmp .end                ; return b
        
.l3:                            ; else
        mov eax, [ebp+8]        ;
        mov eax, [eax+16]       ;
        mov [esp], eax          ; a->r
        mov eax, [ebp+12]       ;
        mov [esp+4], eax        ; b
        call merge              ;
        mov ecx, [ebp+8]        ;
        mov [ecx+16], eax       ; a->r = merge(a->r, b)
        
        mov eax, [ebp+8]        ; return a

.end:
        mov esp, ebp
        pop ebp
        ret

;;; ========= void split(node *t, int k, node *&a, node *&b) ==========
split:
        push ebp
        mov ebp, esp
        sub esp, 24
        
        cmp dword[ebp+8], 0     ; if (!t) then
        jne .l1                 ;
        mov eax, [ebp+16]       ; 
        mov dword[eax], 0       ; a = NULL
        mov eax, [ebp+20]       ;
        mov dword[eax], 0       ; b = NULL
        jmp .end                ; return;
.l1:
        mov ecx, [ebp+8]        ; t
        mov ecx, [ecx]          ; t->x
        
        mov eax, [ebp+12]       ; k
        
        cmp eax, ecx            ; if k <= t->x
        jg .l2                  ; then
        mov eax, [ebp+8]        ; t
        mov eax, [eax+12]       ; t->l
        mov [esp], eax          ;
        mov eax, [ebp+8]        ;
        lea eax, [eax+12]       ;
        mov [esp+12], eax       ; &(t->l)
        mov eax, [ebp+12]       ;
        mov [esp+4], eax        ;
        mov eax, [ebp+16]       ;
        mov [esp+8], eax        ;
        call split              ; split(t->l, k, a, t->l)
        
        mov eax, [ebp+20]       ;
        mov ecx, [ebp+8]        ;
        mov [eax], ecx          ; b = t
        jmp .end
.l2:
        mov eax, [ebp+8]        ; t
        mov eax, [eax+16]       ; t->r
        mov [esp], eax          ;
        mov eax, [ebp+8]        ;
        lea eax, [eax+16]       ; &(t->r)
        mov [esp+8], eax        ;
        mov eax, [ebp+12]       ;
        mov [esp+4], eax        ; k 
        mov eax, [ebp+20]       ;
        mov [esp+12], eax       ;
        call split              ; split(t->r, k, t->r, b)
        
        mov eax, [ebp+16]       ;
        mov ecx, [ebp+8]        ;
        mov [eax], ecx          ; a = t

.end:
        mov esp, ebp
        pop ebp
        ret

        
;;; ============= void insert(int key, int val) ==============
insert:
        push ebp
        mov ebp, esp
        sub esp, 40

        call make_node          ;
        mov [ebp-4], eax        ; node *t1
        call make_node          ;
        mov [ebp-8], eax        ; node *t2
        call make_node          ;
        mov [ebp-12], eax       ; node *t3

        lea eax, [ebp-4]        ;
        mov [esp+8], eax        ; &t1
        lea eax, [ebp-8]        ;
        mov [esp+12], eax       ; &t2
        mov eax, [ebp+8]        ;
        mov [esp+4], eax        ; key
        mov eax, [root]         ;
        mov [esp], eax          ; root
        call split              ; split(root, key, &t1, &t2)

        mov eax, [ebp-8]        ;
        mov [esp], eax          ; t2
        mov eax, [ebp+8]        ;
        inc eax                 ;
        mov [esp+4], eax        ; key+1
        lea eax, [ebp-8]
        mov [esp+8], eax        ; &t2
        lea eax, [ebp-12]       ;
        mov [esp+12], eax       ; &t3
        call split              ; split(t2, key+1, &t2, &t3)

        mov eax, [ebp+8]        ;
        mov [esp], eax          ; key
        mov eax, [ebp+12]       ;
        mov [esp+4], eax        ; val
        call make_node          ; p = make_node(key, val)
        mov ecx, eax

        mov [esp], ecx          ; p
        mov eax, [ebp-12]       ;
        mov [esp+4], eax        ; t3
        call merge              ; merge(p, t3)
        mov ecx, eax

        mov eax, [ebp-4]        ;
        mov [esp], eax          ; t1
        mov [esp+4], ecx        ;
        call merge              ; 
        mov [root], eax         ; root = merge(t1, merge(p, t3))

        mov eax, [ebp-8]        ;
        mov [esp], eax          ;
        call free               ; free(t2)


        mov esp, ebp
        pop ebp
        ret

;;; ======== void clean(node *a) ===========
clean:
        push ebp
        mov ebp, esp
        sub esp, 8

        cmp dword[ebp+8], 0     ; if !a then
        je .end                 ; return

        mov eax, [ebp+8]        ; 
        mov eax, [eax+12]       ;
        mov [esp], eax          ;
        call clean              ; clean(a->l)
        
        mov eax, [ebp+8]        ; 
        mov eax, [eax+16]       ;
        mov [esp], eax          ;
        call clean              ; clean(a->r)

        mov eax, [ebp+8]        ;
        mov [esp], eax          ;
        call free               ; free(a)
        
.end:
        mov esp, ebp
        pop ebp
        ret

;;; ========= int search(int key) =============
search: 
        push ebp
        mov ebp, esp
        sub esp, 40

        call make_node          ;
        mov [ebp-4], eax        ; node *t1
        call make_node          ;
        mov [ebp-8], eax        ; node *t2
        call make_node          ;
        mov [ebp-12], eax       ; node *t3

        lea eax, [ebp-4]        ;
        mov [esp+8], eax        ; &t1
        lea eax, [ebp-8]        ;
        mov [esp+12], eax       ; &t2
        mov eax, [ebp+8]        ;
        mov [esp+4], eax        ; key
        mov eax, [root]         ;
        mov [esp], eax          ; root
        call split              ; split(root, key, &t1, &t2)

        mov eax, [ebp-8]        ;
        mov [esp], eax          ; t2
        mov eax, [ebp+8]        ;
        inc eax                 ;
        mov [esp+4], eax        ; key+1
        lea eax, [ebp-8]
        mov [esp+8], eax        ; &t2
        lea eax, [ebp-12]       ;
        mov [esp+12], eax       ; &t3
        call split              ; split(t2, key+1, &t2, &t3)

        cmp dword[ebp-8], 0     ; if t2 != NULL
        je .l1                  ; then
        mov dword[found], 1     ; found = 1
        mov eax, [ebp-8]        ; 
        mov eax, [eax+8]        ;
        mov [ebp-16], eax       ; ans = t2->v
.l1:

        mov eax, [ebp-8]        ;
        mov [esp], eax          ; t2
        mov eax, [ebp-12]       ;
        mov [esp+4], eax        ; t3
        call merge              ; merge(t2, t3)
        mov ecx, eax

        mov eax, [ebp-4]        ;
        mov [esp], eax          ; t1
        mov [esp+4], ecx        ;
        call merge              ; 
        mov [root], eax         ; root = merge(t1, merge(t2, t3))

        mov eax, [ebp-16]       ; return ans

        mov esp, ebp
        pop ebp
        ret

;;; ========= void erase(int key) =============
erase: 
        push ebp
        mov ebp, esp
        sub esp, 40

        call make_node          ;
        mov [ebp-4], eax        ; node *t1
        call make_node          ;
        mov [ebp-8], eax        ; node *t2
        call make_node          ;
        mov [ebp-12], eax       ; node *t3

        lea eax, [ebp-4]        ;
        mov [esp+8], eax        ; &t1
        lea eax, [ebp-8]        ;
        mov [esp+12], eax       ; &t2
        mov eax, [ebp+8]        ;
        mov [esp+4], eax        ; key
        mov eax, [root]         ;
        mov [esp], eax          ; root
        call split              ; split(root, key, &t1, &t2)

        mov eax, [ebp-8]        ;
        mov [esp], eax          ; t2
        mov eax, [ebp+8]        ;
        inc eax                 ;
        mov [esp+4], eax        ; key+1
        lea eax, [ebp-8]
        mov [esp+8], eax        ; &t2
        lea eax, [ebp-12]       ;
        mov [esp+12], eax       ; &t3
        call split              ; split(t2, key+1, &t2, &t3)

        mov eax, [ebp-4]        ;
        mov [esp], eax          ; t1
        mov eax, [ebp-12]       ;
        mov [esp+4], eax        ; t3
        call merge              ; 
        mov [root], eax         ; root = merge(t1, t3)
        
        mov eax, [ebp-8]        ;
        mov [esp], eax          ;
        call free               ; free(t2)

        mov esp, ebp
        pop ebp
        ret
