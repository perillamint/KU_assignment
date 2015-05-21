void hanoi(int n, stack_t *src, stack_t *dst, stack_t *spare) {
  if (n == 0) {
    dst -> push(src -> pop());
    return;
  }

  hanoi(n - 1, src, spare, dst);
  dst -> push(src -> pop());
  hanoi(n - 1, spare, dst, src);
}
