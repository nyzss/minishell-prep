/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okoca <okoca@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 15:03:36 by okoca             #+#    #+#             */
/*   Updated: 2024/07/04 16:10:25 by okoca            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <stdlib.h>
# include <unistd.h>
# include "ft_printf.h"
# include "get_next_line.h"
# define FIRST 0
# define SECOND 0

int		ft_isupper(int c);

int		ft_islower(int c);

/*
* Joins two strings into a new string, optionally freeing the original strings.
*/
char	*ft_strjoin_free(char *first, char *second, int to_free);


/*
* Joins two strings with an additional string in between.
*/
char	*ft_strjoin_add(char *first, char *second, char *add);

/*
* Appends a copy of the source string to the destination string.
*/
char	*ft_strcat(char *dest, char *src);

/*
* Creates a duplicate of the string, with an option to free the original.
*/
char	*ft_strdup_free(char *str);

/*
* Duplicates the first n bytes of a string.
*/
char	*ft_strndup(const char *str, int nb);

/*
* Compares two strings and returns an integer less than, equal to, or greater than zero.
*/
int		ft_strcmp(const char *s1, const char *s2);

/*
* Reallocates memory area, possibly moving it.
* Has to take a NULL-Terminated array for it to work.
*/
void	*ft_realloc(void *ptr, size_t size);

/*
* Checks for an alphabetic character.
*/
int		ft_isalpha(int c);

/*
* Checks for a digit (0 through 9).
*/
int		ft_isdigit(int c);

/*
* Checks for an alphanumeric character.
*/
int		ft_isalnum(int c);

/*
* Checks whether c is a 7-bit unsigned char value that fits into the ASCII character set.
*/
int		ft_isascii(int c);

/*
* Checks for any printable character including space.
*/
int		ft_isprint(int c);

/*
* Converts a character to lowercase.
*/
int		ft_tolower(int c);

/*
* Converts a character to uppercase.
*/
int		ft_toupper(int c);

/*
* Converts a string to an integer.
*/
int		ft_atoi(const char *str);

/*
* Compares up to n characters of two strings.
*/
int		ft_strncmp(const char *s1, const char *s2, size_t n);

/*
* Creates a duplicate of a string.
*/
char	*ft_strdup(const char *str);

/*
* Locates a substring in a string, searching only the first n bytes.
*/
char	*ft_strnstr(const char *big, const char *little, size_t len);

/*
* Joins two strings into a new string.
*/
char	*ft_strjoin(char *s1, char *s2);

/*
* Locates the first occurrence of c (converted to a char) in the string pointed to by str.
*/
char	*ft_strchr(const char *str, int c);

/*
* Locates the last occurrence of c (converted to a char) in the string pointed to by str.
*/
char	*ft_strrchr(const char *str, int c);

/*
* Copies up to size - 1 characters from the NUL-terminated string src to dst, NUL-terminating the result.
*/
size_t	ft_strlcpy(char *dst, const char *src, size_t size);

/*
* Appends string src to the end of dst. It will append at most size - strlen(dst) - 1 bytes, NUL-terminating the result.
*/
size_t	ft_strlcat(char *dst, const char *src, size_t size);

/*
* Calculates the length of the string pointed to by str, excluding the terminating null byte ('\0').
*/
size_t	ft_strlen(const char *str);

/*
* Fills the first n bytes of the memory area pointed to by addr with the constant byte c.
*/
void	*ft_memset(void *addr, int c, size_t n);

/*
* Erases the data in the n bytes of the memory starting at the location pointed to by addr by writing zeros (bytes containing '\0').
*/
void	ft_bzero(void *addr, size_t n);

/*
* Copies n bytes from memory area src to memory area dest. The memory areas must not overlap.
*/
void	*ft_memcpy(void *dest, const void *src, size_t n);

/*
* Moves n bytes from memory area src to memory area dest. The memory areas may overlap.
*/
void	*ft_memmove(void *dest, const void *src, size_t n);

/*
* Scans the initial n bytes of the memory area pointed to by s for the first instance of c.
*/
void	*ft_memchr(const void *s, int c, size_t n);

/*
* Compares the first n bytes of the memory areas s1 and s2.
*/
int		ft_memcmp(const void *s1, const void *s2, size_t n);

/*
* Allocates memory for an array of n elements of size bytes each and returns a pointer to the allocated memory.
*/
void	*ft_calloc(size_t n, size_t size);

/*
* Trims the beginning and end of str of any of the characters contained in set.
*/
char	*ft_strtrim(char const *str, char const *set);

/*
* Allocates and returns a substring from the string s.
*/
char	*ft_substr(char const *s, unsigned int start, size_t len);

/*
* Splits a string into an array of strings using a character as the delimiter.
*/
char	**ft_split(char const *str, char c);

/*
* Converts an integer value to a null-terminated string.
*/
char	*ft_itoa(int n);

/*
* Applies the function f to each character of the string passed as argument, and passing its index as first argument.
*/
void	ft_striteri(char *str, void (*f)(unsigned int, char *));

/*
* Applies the function f to each character of the string s to create a new string (with malloc(3)) resulting from successive applications of f.
*/
char	*ft_strmapi(char const *str, char (*f)(unsigned int, char));

/*
* Outputs the character c to the given file descriptor.
*/
void	ft_putchar_fd(char c, int fd);

/*
* Outputs the string s to the given file descriptor.
*/
void	ft_putstr_fd(char *s, int fd);

/*
* Outputs the string s to the given file descriptor, followed by a newline.
*/
void	ft_putendl_fd(char *s, int fd);

/*
* Outputs the integer n to the given file descriptor.
*/
void	ft_putnbr_fd(int n, int fd);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);

int		ft_lstsize(t_list *lst);

void	ft_lstadd_front(t_list **lst, t_list *new);

t_list	*ft_lstlast(t_list *lst);

void	ft_lstadd_back(t_list **lst, t_list *new);

void	ft_lstdelone(t_list *lst, void (*del)(void*));

void	ft_lstclear(t_list **lst, void (*del)(void*));

void	ft_lstiter(t_list *lst, void (*f)(void *));

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
