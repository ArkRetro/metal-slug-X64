/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01bfd0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1bfd0(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  char cVar5;
  short sVar6;
  int unaff_D2;
  int unaff_D3;
  undefined2 unaff_D4w;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined8 uVar7;

  cVar5 = candidate_49e8();
  if (cVar5 == '\0') {
    unaff_A6[0x1e] = unaff_A6[0xc];
    unaff_A6[0x1f] = unaff_A6[0xd];
    func_0x000143e4();
    *(undefined2 *)((int)unaff_A6 + 0x76) = unaff_D4w;
    if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) ||
       ((((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0 &&
         ((*(byte *)((int)unaff_A6 + 0x5f) & 2) == 0)) &&
        ((*(byte *)((int)unaff_A6 + 0x5f) & 1) == 0)))) {
      candidate_505a();
      uVar7 = candidate_1c6ac();
      *(undefined8 *)(unaff_A6 + 0xc) = uVar7;
      sVar4 = (ushort)*(byte *)((int)unaff_A6 + 0x76) * 2;
      sVar6 = *(short *)(unaff_A6 + 0x1d);
      *(short *)(unaff_A6 + 0x1d) =
           ((short)((short)((int)(short)((uint)unaff_D2 >> 8) * (int)*(short *)(sVar4 + 0x29fc) -
                            (int)(short)((uint)unaff_D3 >> 8) * (int)*(short *)(sVar4 + 0x2a7c) >>
                           0xb) - sVar6) >> 1) + *(short *)(unaff_A6 + 0x1d);
      sVar6 = sVar6 << 3;
      sVar3 = *(short *)(sVar4 + 0x2a7c);
      unaff_A6[0xc] = ((int)sVar6 * (int)*(short *)(sVar4 + 0x29fc) - unaff_D2 >> 1) + unaff_A6[0xc]
      ;
      unaff_A6[0xd] = (-unaff_D3 - (int)sVar6 * (int)sVar3 >> 1) + unaff_A6[0xd];
      sVar6 = *(short *)(unaff_A6 + 0x1c);
      *(short *)(unaff_A6 + 0x1c) = sVar6 + -1;
      if (sVar6 == 0) goto LAB_0001c10c;
    }
    unaff_A6[0xd] = ((int)(unaff_A6[0x21] - unaff_A6[0xd]) >> 4) + unaff_A6[0xd];
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 4);
    sVar6 = *(short *)(unaff_A6 + 0x1d);
    *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) - (sVar6 >> 5);
    *(ushort *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + sVar6 & 0x1fff;
    candidate_427a();
    if ((*(ushort *)(unaff_A6 + 0xf) < 0x140) &&
       ((ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U) < 0x140)) {
      return;
    }
    candidate_c0d48();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
LAB_0001c10c:
  candidate_c0d48();
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = candidate_1c152;
  return;
}
