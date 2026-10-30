/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ddd4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ddd4(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_9ea8();
  candidate_1b1dc();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  sVar1 = *(short *)((int)unaff_A6 + 0x76);
  if (sVar1 == 0) {
    sVar1 = candidate_260c8();
  }
  *(short *)(unaff_A6 + 0x1e) = sVar1;
  candidate_4256();
  *unaff_A6 = 0x7de22;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_7de3c;
  }
  *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
  if (*(short *)(unaff_A6 + 0xf) < 1) {
    *unaff_A6 = candidate_7e09e;
  }
  return;
}
