/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x053dcc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_53dcc(void)

{
  int iVar1;
  short in_D1w;
  int unaff_A6;

  if (*(byte *)(unaff_A6 + 0x86) < 0x20) {
    *(char *)(unaff_A6 + 0x86) = *(char *)(unaff_A6 + 0x86) + '\a';
  }
  if (*(char *)(unaff_A6 + 0x86) == '\a') {
    in_D1w = 0x3bf;
  }
  if (*(char *)(unaff_A6 + 0x86) == '\x0e') {
    in_D1w = 0x3c0;
  }
  if (*(char *)(unaff_A6 + 0x86) == '\x15') {
    in_D1w = 0x3c1;
  }
  if (*(char *)(unaff_A6 + 0x86) == '\x1c') {
    in_D1w = 0x3c2;
  }
  iVar1 = (int)(short)((ushort)*(byte *)(unaff_A6 + 0x14) << 4);
  if (*(short *)(iVar1 + 0x1068f2) == 0) {
    return;
  }
  if (in_D1w == *(short *)(iVar1 + 0x1068f6)) {
    if ((*(char *)(iVar1 + 0x1068f8) != ' ') &&
       (*(undefined1 *)(iVar1 + 0x1068f8) = 0x20, in_D1w != *(short *)(iVar1 + 0x1068f4))) {
      *(byte *)(iVar1 + 0x1068f9) = *(byte *)(iVar1 + 0x1068f9) | 0x80;
    }
  }
  else {
    *(short *)(iVar1 + 0x1068f6) = in_D1w;
    *(undefined1 *)(iVar1 + 0x1068f8) = 0x20;
    *(byte *)(iVar1 + 0x1068f9) = *(byte *)(iVar1 + 0x1068f9) | 0x80;
  }
  return;
}
