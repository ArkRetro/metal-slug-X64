/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00454c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_454c(void)

{
  int iVar1;
  short in_D0w;
  short in_D1w;
  int unaff_A6;

  if ((char)*(byte *)(unaff_A6 + 0x17) >> 1 < '\0') {
    return;
  }
  if ((*(byte *)(unaff_A6 + 0x17) & 1) == 0) {
    in_D1w = in_D0w;
  }
  iVar1 = (int)(short)((ushort)*(byte *)(unaff_A6 + 0x16) << 4);
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
