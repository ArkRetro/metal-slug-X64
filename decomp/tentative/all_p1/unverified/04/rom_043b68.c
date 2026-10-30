/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043b68.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_43b68(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int unaff_A6;
  bool bVar3;

  if (*(short *)(unaff_A6 + 0x8e) == 0) {
    if ((*(char *)(unaff_A6 + 0x80) != '\x05') ||
       (sVar1 = *(short *)(unaff_A6 + 0x90) + -1, *(short *)(unaff_A6 + 0x90) = sVar1, sVar1 < 0)) {
LAB_00043bae:
      *(undefined2 *)(unaff_A6 + 0x90) = 0x5a;
      return;
    }
    bVar3 = false;
    candidate_1aeb0();
    if (bVar3) {
      uVar2 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if ((short)uVar2 < 0) {
        uVar2 = -uVar2;
      }
      if (0x2f < uVar2) goto LAB_00043bae;
    }
  }
  else {
    *(short *)(unaff_A6 + 0x8e) = *(short *)(unaff_A6 + 0x8e) + -1;
  }
  return;
}
