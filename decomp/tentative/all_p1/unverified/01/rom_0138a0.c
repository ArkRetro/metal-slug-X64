/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0138a0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_138a0(void)

{
  int in_D0;
  int in_D1;
  int unaff_D2;
  int unaff_D3;
  int iVar1;
  int unaff_A5;
  bool bVar2;

  if (*(char *)(unaff_A5 + 0x400d) == '\0') {
    return;
  }
  if (*(char *)(unaff_A5 + 0x400b) != '\0') {
    return;
  }
  if (unaff_D2 != 0) {
    if (unaff_D2 < 0) {
      iVar1 = (uint)*(ushort *)(unaff_A5 + 0x401c) * 0x10000 - in_D0;
      if (-1 < iVar1) {
        iVar1 = 0;
      }
      if (unaff_D2 < iVar1) {
        *(undefined1 *)(unaff_A5 + 0x4025) = 0xff;
      }
    }
    else {
      iVar1 = ((uint)(ushort)(*(short *)(unaff_A5 + 0x4020) + *(short *)(unaff_A5 + 0x401c)) *
               0x10000 + -1) - in_D0;
      if (iVar1 < 0) {
        iVar1 = 0;
      }
      if (iVar1 < unaff_D2) {
        *(undefined1 *)(unaff_A5 + 0x4024) = 0xff;
      }
    }
  }
  if ((*(char *)(unaff_A5 + 0x400c) == '\0') && (unaff_D3 != 0)) {
    if (unaff_D3 < 0) {
      iVar1 = ((uint)*(ushort *)(unaff_A5 + 0x401e) * 0x10000 - *(int *)(unaff_A5 + 0x69cc)) - in_D1
      ;
      bVar2 = unaff_D3 - iVar1 < 0;
      if ((bVar2 && SBORROW4(unaff_D3,iVar1)) != bVar2) {
        *(undefined1 *)(unaff_A5 + 0x4027) = 0xff;
      }
    }
    else {
      iVar1 = (((uint)(ushort)(*(short *)(unaff_A5 + 0x4022) + *(short *)(unaff_A5 + 0x401e)) *
                0x10000 - *(int *)(unaff_A5 + 0x69cc)) + -1) - in_D1;
      if (iVar1 < 0) {
        iVar1 = 0;
      }
      if (iVar1 < unaff_D3) {
        *(undefined1 *)(unaff_A5 + 0x4026) = 0xff;
      }
    }
  }
  return;
}
