/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0684ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_684ca(void)

{
  char cVar1;
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar2;

  candidate_1456a();
  candidate_427a();
  candidate_68cd4();
  if ((bool)in_CF) {
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
    candidate_1af9a();
    *(undefined4 *)((int)unaff_A6 + 0x86) = extraout_A0;
    *unaff_A6 = 0x67ee4;
    in_CF = 0;
  }
  candidate_68c3a();
  if ((bool)in_CF) {
    *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
    candidate_1af9a();
    *(undefined4 *)((int)unaff_A6 + 0x86) = extraout_A0_00;
    *unaff_A6 = 0x67e66;
  }
  candidate_68f66();
  bVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x9a) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9a);
    bVar2 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9a) = cVar1 + -1;
  }
  candidate_4c24();
  if (bVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      candidate_6914a();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_6918e();
  }
  candidate_688d6();
  return;
}
