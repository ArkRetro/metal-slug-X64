/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ca5c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ca5c(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x96;
  *(short *)(unaff_A6[3] + 0x7c) = *(short *)(unaff_A6[3] + 0x7c) + -1;
  *unaff_A6 = 0x8ca7a;
  sVar1 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1;
  if ((sVar1 == 0) || (*(char *)(unaff_A6[3] + 0x89) == '\0')) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
    unaff_A6[0xd] = 0;
    *unaff_A6 = candidate_8caa0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x85) & 2) == 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  }
  candidate_1ac38();
  candidate_427a();
  *(char *)((int)unaff_A6 + 0x85) = *(char *)((int)unaff_A6 + 0x85) + '\x01';
  return;
}
