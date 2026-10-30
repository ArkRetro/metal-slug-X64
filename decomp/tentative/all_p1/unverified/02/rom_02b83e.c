/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02b83e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_2b83e(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_9ea8();
  candidate_9ea8();
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 8;
  if (*(short *)(unaff_A6 + 0xb) < 0x179) {
    unaff_A6[0xd] = 0x5b05b;
    *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xffffdfa3;
    unaff_A6[0xc] = 0xb60b;
  }
  else {
    unaff_A6[0xd] = 0x22222;
    *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xffffdb98;
    unaff_A6[0xc] = 0x22222;
  }
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *unaff_A6 = 0x2b8d4;
  candidate_2bdd8();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    candidate_120f8();
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    *unaff_A6 = candidate_2b822;
  }
  *(undefined1 *)(unaff_A6 + 5) =
       *(undefined1 *)
        ((int)unaff_A6 + (short)(*(byte *)((short)(_DAT_0010000e >> 1 & 3) + 0x2c546) & 3) + 0x19);
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_2b822;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    *unaff_A6 = candidate_2b822;
  }
  bVar2 = false;
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
