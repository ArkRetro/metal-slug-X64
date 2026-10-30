/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0973e2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_973e2(void)

{
  undefined2 *extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
  candidate_1b1dc();
  *(undefined1 *)(unaff_A6 + 0x21) = 1;
  candidate_966f0();
  candidate_120f8();
  candidate_353c2();
  *unaff_A6 = 0x97414;
  candidate_ca2ce();
  *(char *)((int)unaff_A6 + 0x85) = (char)*extraout_A0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  uVar1 = 0;
  if (*(short *)((int)unaff_A6 + 0x6e) < 0) {
    func_0x000143e4();
  }
  else {
    if ((*(byte *)((int)unaff_A6 + 0x85) & 0x20) != 0) {
      candidate_aa7fc();
    }
    uVar1 = false;
    candidate_aa760();
    if (!(bool)uVar1) {
      *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xffff;
      candidate_966f0();
      *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
      uVar1 = 0;
    }
    candidate_143ee();
  }
  candidate_427a();
  candidate_97ede();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_9756a;
  }
  return;
}
