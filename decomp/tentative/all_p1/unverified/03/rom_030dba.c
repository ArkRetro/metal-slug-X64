/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x030dba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_30dba(void)

{
  char cVar1;
  undefined4 *unaff_A6;

  candidate_3035e();
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 4) != 0) {
    candidate_307d6();
    candidate_120f8();
  }
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0x49;
  *unaff_A6 = 0x30de2;
  cVar1 = *(char *)(unaff_A6[3] + 0x61);
  *(char *)((int)unaff_A6 + 0x61) = cVar1;
  if (cVar1 != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x7e) = 1;
  }
  *(short *)((int)unaff_A6 + 0x7e) = *(short *)((int)unaff_A6 + 0x7e) + -1;
  if (*(short *)((int)unaff_A6 + 0x7e) < 1) {
    if ((*(byte *)((int)unaff_A6 + 0x8f) & 4) != 0) {
      candidate_30460();
      candidate_303de();
      candidate_120f8();
    }
    *unaff_A6 = candidate_30e3a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x8f) & 8) == 0) {
    *unaff_A6 = candidate_30e3a;
  }
  *(char *)(unaff_A6[3] + 0x60) = *(char *)(unaff_A6[3] + 0x60) + '\x01';
  candidate_427a();
  return;
}
