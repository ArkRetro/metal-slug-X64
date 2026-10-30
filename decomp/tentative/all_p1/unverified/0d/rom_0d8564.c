/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8564.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8564(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0x14] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  *(undefined2 *)(unaff_A6 + 0x1f) = 8;
  candidate_4256();
  *unaff_A6 = 0xd8590;
  candidate_d9276();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + 1;
  bVar2 = false;
  uVar1 = *(ushort *)((int)unaff_A6 + 0x76) & 1;
  *(ushort *)((int)unaff_A6 + 0x76) = uVar1;
  if (uVar1 == 0) {
    candidate_d9242();
    if ((bVar2) && (*(char *)((int)unaff_A6 + 0x8a) == '\0')) {
      *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
      candidate_d922c();
    }
  }
  candidate_d92ec();
  return;
}
