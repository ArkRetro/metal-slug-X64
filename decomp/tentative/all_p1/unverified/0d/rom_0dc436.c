/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dc436.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dc436(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x3c;
  candidate_4256();
  *unaff_A6 = 0xdc458;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_4c24();
    if ((bool)uVar1) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      candidate_40bc();
      candidate_1acf2();
      candidate_4256();
    }
  }
  candidate_dc79c();
  return;
}
