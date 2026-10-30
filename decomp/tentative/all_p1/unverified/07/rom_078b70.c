/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x078b70.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_78b70(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x78b8c;
  uVar1 = 0;
  candidate_78cf4();
  candidate_427a();
  if ((bool)uVar1) {
    if (*(char *)(unaff_A6 + 0x27) == '\0') {
      candidate_1ad52();
      *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
      *(undefined2 *)(unaff_A6 + 0x1c) = 0xffa0;
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
        *(short *)(unaff_A6 + 0x1c) = -*(short *)(unaff_A6 + 0x1c);
      }
      *(undefined2 *)((int)unaff_A6 + 0x72) = 0xffff;
      *(undefined2 *)(unaff_A6 + 0x1d) = 1;
      *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
      *unaff_A6 = candidate_99374;
      return;
    }
    *unaff_A6 = candidate_78bd2;
    uVar1 = 0;
  }
  candidate_78fc4();
  candidate_78fa2();
  candidate_78d54();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
    uVar1 = 0;
  }
  candidate_78d7a();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
