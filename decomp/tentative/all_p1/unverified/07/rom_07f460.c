/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f460.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f460(void)

{
  undefined4 *unaff_A6;
  bool bVar1;
  undefined1 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x8820;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x8820;
  bVar1 = false;
  candidate_1b2be();
  if (bVar1) {
    candidate_9ea8();
  }
  else {
    candidate_9ea8();
  }
  candidate_1ad6e();
  candidate_1aca2();
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x7f4c2;
  if (*(char *)(unaff_A6[3] + 0x85) == '\x01') {
    candidate_4256();
    *unaff_A6 = 0x7f502;
    candidate_1abc8();
    candidate_427a();
    if (*(char *)(unaff_A6[3] + 0x85) != '\x02') {
      if (*(char *)(unaff_A6[3] + 0x85) == '\x03') {
        *unaff_A6 = 0x7f712;
      }
      candidate_7f7a0();
      return;
    }
  }
  else if (*(char *)(unaff_A6[3] + 0x85) != '\x02') {
    candidate_1abc8();
    candidate_427a();
    candidate_7f7a0();
    return;
  }
  candidate_4256();
  *unaff_A6 = 0x7f548;
  uVar2 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = 0x7f712;
  }
  candidate_7f7a0();
  return;
}
