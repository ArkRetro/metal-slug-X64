/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c968.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c968(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_7ccec();
  candidate_4256();
  *unaff_A6 = 0x7c9a2;
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_7c9f0;
  }
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_7c94e;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
