/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06936e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6936e(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    func_0x000143e4();
    candidate_427a();
  }
  else {
    candidate_69596();
    if (*(char *)((int)unaff_A6 + 0x9d) < '\0') {
      candidate_4256();
    }
    else {
      candidate_4256();
    }
    candidate_4e76();
    *unaff_A6 = 0x693ca;
    uVar2 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_693e4;
    }
  }
  bVar3 = false;
  if (*(char *)((int)unaff_A6 + 0x9a) != '\0') {
    cVar1 = *(char *)((int)unaff_A6 + 0x9a);
    bVar3 = cVar1 == '\0';
    *(char *)((int)unaff_A6 + 0x9a) = cVar1 + -1;
  }
  candidate_4c24();
  if (bVar3) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
        candidate_69448();
      }
      else {
        candidate_69582();
        candidate_6914a();
      }
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_6918e();
  }
  candidate_688d6();
  return;
}
