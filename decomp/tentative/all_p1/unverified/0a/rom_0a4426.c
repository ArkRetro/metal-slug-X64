/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4426.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_a4426(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int unaff_A5;
  int unaff_A6;

  uVar2 = candidate_190bc();
  uVar3 = uVar2 & 0xffff7070;
  if ((uVar2 & 0x7070) == 0) {
    *(undefined1 *)(unaff_A5 + 0x4604) = 0;
  }
  else {
    *(undefined1 *)(unaff_A5 + 0x4604) = 0xff;
  }
  if (*(char *)(unaff_A6 + 0x61) == '\0') {
    if (*(int *)(unaff_A6 + 0x80) == 0) {
      uVar3 = candidate_120f8();
      *(undefined1 *)(unaff_A6 + 0x61) = 0xff;
    }
    else {
      uVar3 = candidate_a49b0();
      iVar1 = *(int *)(unaff_A6 + 0x80) - uVar3;
      *(int *)(unaff_A6 + 0x80) = iVar1;
      if (iVar1 < 0) {
        *(undefined4 *)(unaff_A6 + 0x80) = 0;
      }
    }
  }
  return uVar3;
}
