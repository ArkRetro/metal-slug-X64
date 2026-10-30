/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d45ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d45ee(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  if (*(char *)((int)unaff_A6 + 0x92) == '\x04') {
    bVar2 = false;
    candidate_40bc();
    if (bVar2) {
      candidate_1b220();
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x92) = 5;
  candidate_120f8();
  candidate_4256();
  *unaff_A6 = 0xd4640;
  uVar3 = 0;
  candidate_d5506();
  candidate_d54ee();
  candidate_427a();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_d4678;
  }
  cVar1 = *(char *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e));
  *(undefined1 *)(unaff_A6[3] + (int)*(short *)((int)unaff_A6 + 0x8e)) = 0;
  if (cVar1 == '\x05') {
    *unaff_A6 = candidate_d470e;
  }
  return;
}
