/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03e5d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3e5d6(void)

{
  undefined1 uVar2;
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined8 uVar3;

  uVar2 = candidate_29c6();
  *(undefined1 *)(unaff_A6 + 0x22) = uVar2;
  candidate_29c6();
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar3;
  uVar1 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1d) = (uVar1 & 3) + 2;
  if ((int)unaff_A6[0xc] < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *unaff_A6 = 0x3e638;
  candidate_3fa46();
  candidate_1abc8();
  candidate_1ad8c();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_3e5d6;
  }
  candidate_427a();
  candidate_3fada();
  return;
}
