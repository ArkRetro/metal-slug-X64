/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4698.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4698(void)

{
  undefined4 uVar1;
  short sVar2;
  int unaff_A6;

  candidate_a4a1c();
  candidate_1ac38();
  uVar1 = *(undefined4 *)(unaff_A6 + 0x3c);
  sVar2 = candidate_a489a();
  *(short *)(unaff_A6 + 0x3c) = (sVar2 >> 8) + *(short *)(unaff_A6 + 0x3c);
  candidate_427a();
  *(undefined4 *)(unaff_A6 + 0x3c) = uVar1;
  *(int *)(unaff_A6 + 0xa4) = *(int *)(unaff_A6 + 0x84) + *(int *)(unaff_A6 + 0xa4);
  sVar2 = candidate_a489a();
  if (sVar2 < 0) {
    sVar2 = -sVar2;
  }
  *(char *)(unaff_A6 + 0x12) = (char)sVar2;
  *(int *)(unaff_A6 + 0x9c) = *(int *)(unaff_A6 + 0x8c) + *(int *)(unaff_A6 + 0x9c);
  if (0x7fffff < *(uint *)(unaff_A6 + 0x9c)) {
    *(uint *)(unaff_A6 + 0x9c) = *(uint *)(unaff_A6 + 0x9c) & 0x7fffff;
    *(byte *)(unaff_A6 + 0x15) = *(byte *)(unaff_A6 + 0x15) ^ 1;
  }
  if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x19);
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x14) = *(undefined1 *)(unaff_A6 + 0x1a);
  }
  candidate_a472c();
  return;
}
