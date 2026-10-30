/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cba96.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cba96(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined2 uVar3;
  undefined4 *unaff_A6;
  bool bVar4;
  undefined1 uVar5;

  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  uVar1 = *(ushort *)(unaff_A6[3] + 0x7c);
  uVar2 = *(ushort *)(unaff_A6[3] + 0x4e);
  bVar4 = uVar1 < uVar2;
  if ((short)uVar2 <= (short)uVar1) {
    *(code **)((int)unaff_A6 + 0x7e) = candidate_cba96;
    bVar4 = false;
    if (*(char *)((int)unaff_A6 + 0x82) == '\0') {
      uVar3 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar3;
      *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
      *unaff_A6 = candidate_cba3a;
      candidate_cb878();
      candidate_427a();
      *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
      if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
        *(undefined1 *)((int)unaff_A6 + 0x82) = 1;
        *unaff_A6 = *(undefined4 *)((int)unaff_A6 + 0x7e);
      }
      goto LAB_000cb90c;
    }
  }
  candidate_1affc();
  if (bVar4) {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x1c) = uVar3;
    candidate_4256();
    candidate_cb8d8();
    *unaff_A6 = 0xcbb00;
    uVar5 = 0;
    candidate_cb878();
    candidate_427a();
    if ((bool)uVar5) {
      if (*(short *)(unaff_A6 + 0x1c) == 0) {
        *unaff_A6 = candidate_cbb32;
      }
      else if (*(short *)((int)unaff_A6 + 0x6e) == 0) {
        *unaff_A6 = 0xcbae6;
      }
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0xcbcc6;
    uVar5 = 0;
    candidate_cb878();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_cbcde;
    }
  }
LAB_000cb90c:
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cbd5c;
  }
  return;
}
