/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a8df6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a8df6(void)

{
  short sVar1;
  char cVar2;
  undefined4 *unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x22) = 0x32;
  unaff_A6[0x14] = 0;
  candidate_4256();
  *unaff_A6 = 0xa8e1a;
  if (*(char *)((int)unaff_A6 + 0x95) == '\0') {
    candidate_1436a();
  }
  else {
    candidate_1abc8();
  }
  candidate_427a();
  sVar1 = *(short *)(unaff_A6 + 0x22) + -1;
  *(short *)(unaff_A6 + 0x22) = sVar1;
  if (sVar1 == 0) {
    *unaff_A6 = candidate_a8e8e;
  }
  if (*(char *)((int)unaff_A6 + 0x93) == '\0') {
    cVar2 = candidate_19dca();
    if (cVar2 == '\0') {
      *unaff_A6 = candidate_a8fe0;
    }
  }
  else if (((*(byte *)(unaff_A6 + 0x18) & 0x80) != 0) && (cVar2 = candidate_19dca(), cVar2 == '\0'))
  {
    *unaff_A6 = candidate_a8fe0;
  }
  candidate_a9a56();
  return;
}
