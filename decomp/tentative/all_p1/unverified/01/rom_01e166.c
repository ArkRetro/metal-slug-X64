/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01e166.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1e166(void)

{
  undefined2 in_D0w;
  undefined2 in_D1w;
  short unaff_D2w;
  char unaff_D3b;
  undefined2 *puVar1;

  if (unaff_D2w == 0) {
    puVar1 = (undefined2 *)0x10c340;
  }
  else {
    puVar1 = (undefined2 *)0x10c348;
  }
  *puVar1 = in_D0w;
  puVar1[1] = in_D1w;
  if ((unaff_D3b != *(char *)(puVar1 + 2)) &&
     (*(char *)(puVar1 + 2) = unaff_D3b, *(char *)((int)puVar1 + 5) < '\x01')) {
    *(undefined1 *)(puVar1 + 3) = 0;
  }
  return;
}
