/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8af4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8af4(void)

{
  ushort uVar1;
  ushort *puVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  uVar1 = candidate_29c6();
  puVar2 = (ushort *)
           ((int)(short)((*(ushort *)((int)unaff_A6 + 0xaa) & 0xf) * 6) +
           *(int *)((short)((uVar1 & 3) << 2) + 0xe99fc));
  unaff_A6[0xc] = (int)((uint)*puVar2 << 0x10) >> 8;
  *(ushort *)(unaff_A6 + 0x1c) = puVar2[1] * 2;
  unaff_A6[0xd] = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0;
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_4256();
  *unaff_A6 = 0xd8b6c;
  unaff_A6[0xd] = ((int)((uint)*(ushort *)(unaff_A6 + 0x1c) << 0x10) >> 8) + unaff_A6[0xd];
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_41ac;
  }
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + 1;
  uVar3 = *(ushort *)((int)unaff_A6 + 0x76) < 8;
  if (7 < (short)*(ushort *)((int)unaff_A6 + 0x76)) {
    if (0xe < *(short *)((int)unaff_A6 + 0x76)) {
      *unaff_A6 = candidate_41ac;
    }
    uVar3 = false;
    if ((*(ushort *)((int)unaff_A6 + 0x76) & 1) == 0) goto LAB_000d8bc0;
  }
  candidate_427a();
LAB_000d8bc0:
  candidate_19504();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
