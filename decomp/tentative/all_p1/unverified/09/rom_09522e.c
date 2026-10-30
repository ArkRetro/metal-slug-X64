/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x09522e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9522e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined8 uVar2;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_4256();
  unaff_A6[0xc] = *(undefined4 *)(unaff_A6[3] + 0x30);
  unaff_A6[0xd] = *(undefined4 *)(unaff_A6[3] + 0x34);
  uVar2 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x1c) = uVar2;
  sVar1 = candidate_29c6();
  if (sVar1 < 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  if ((char)sVar1 < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
  }
  candidate_1abc8();
  *unaff_A6 = candidate_952b8;
  return;
}
