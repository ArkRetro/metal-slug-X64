/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x086850.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_86850(void)

{
  char cVar1;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  uVar3 = 0;
  candidate_1436a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    if (-1 < *(int *)((int)unaff_A6 + 0x72)) {
      *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffffb000;
    }
    candidate_4256();
    unaff_A6[0x14] = 0x87c9e;
    *unaff_A6 = 0x86ee6;
    candidate_87892();
    candidate_427a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_86f04;
    }
    goto LAB_0008713e;
  }
  candidate_1b284();
  if ((bool)uVar3) {
    unaff_A6[0x14] = 0x87c92;
    candidate_1b1dc();
    candidate_4256();
    *unaff_A6 = 0x8695c;
    candidate_1436a();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      iVar2 = unaff_A6[0xd];
      if (-0x20000 < iVar2) {
        unaff_A6[0xd] = 0xfffe0000;
      }
      uVar3 = iVar2 < -0x20000;
      candidate_143dc();
    }
    else {
      unaff_A6[0xc] = 0;
      unaff_A6[0xd] = 0;
      uVar3 = 0;
    }
    candidate_427a();
    candidate_1b284();
    if (!(bool)uVar3) {
      *unaff_A6 = candidate_86a1c;
    }
  }
  else {
    unaff_A6[0x14] = 0x87c92;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
      *(undefined1 *)((int)unaff_A6 + 0x86) = 0x10;
    }
    if ((*(byte *)((int)unaff_A6 + 0x8b) & 2) == 0) {
      *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
      candidate_1b1dc();
      iVar2 = *(int *)((int)unaff_A6 + 0x76);
      if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
        iVar2 = -iVar2;
      }
      *(int *)((int)unaff_A6 + 0x62) = iVar2;
      candidate_4256();
      *unaff_A6 = 0x86a72;
      candidate_1436a();
      candidate_427a();
      candidate_878f0();
      candidate_8790a();
      if (0x110 < *(short *)(unaff_A6 + 0xf)) {
        *unaff_A6 = candidate_86cf8;
      }
      uVar3 = false;
      if (*(char *)((int)unaff_A6 + 0x86) == '\0') {
        candidate_87998();
        if ((bool)uVar3) {
          *unaff_A6 = candidate_86b52;
          uVar3 = 0;
        }
      }
      else {
        cVar1 = *(char *)((int)unaff_A6 + 0x86);
        uVar3 = cVar1 == '\0';
        *(char *)((int)unaff_A6 + 0x86) = cVar1 + -1;
      }
      candidate_879b0();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_86c3a;
        uVar3 = 0;
      }
      candidate_87a64();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_86d08;
        uVar3 = 0;
      }
      candidate_87aa0();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_86cd8;
      }
      if (*(short *)(unaff_A6 + 0xf) < 0x30) {
        *unaff_A6 = candidate_86cf8;
      }
      uVar3 = false;
      candidate_87ae6();
      if ((bool)uVar3) {
        *unaff_A6 = candidate_86f4c;
        uVar3 = 0;
      }
      candidate_87b64();
      if ((bool)uVar3) {
        *unaff_A6 = 0x86b0e;
      }
    }
    else {
      unaff_A6[0x14] = 0x87c92;
      if ((*(byte *)((int)unaff_A6 + 0x8b) & 4) == 0) {
        unaff_A6[0x14] = 0x87c92;
        if (*(short *)((int)unaff_A6 + 0x7e) < *(short *)(unaff_A6 + 0xf)) {
          if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
LAB_00086b0e:
            unaff_A6[0x14] = 0x87c92;
            candidate_4256();
            *(undefined1 *)((int)unaff_A6 + 0x87) = 6;
            *unaff_A6 = 0x86b32;
            uVar3 = 0;
            candidate_1436a();
            candidate_427a();
            if ((bool)uVar3) {
              *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) ^ 1;
              *unaff_A6 = candidate_86a1c;
            }
            goto LAB_0008712e;
          }
        }
        else if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) goto LAB_00086b0e;
        iVar2 = *(int *)((int)unaff_A6 + 0x76);
        if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
          iVar2 = -iVar2;
        }
        *(int *)((int)unaff_A6 + 0x62) = iVar2;
        candidate_4256();
        *unaff_A6 = 0x87110;
        uVar3 = 0;
        candidate_1456a();
        candidate_427a();
        candidate_87b3a();
        if ((bool)uVar3) {
          *unaff_A6 = candidate_87066;
        }
      }
      else {
        candidate_4256();
        *unaff_A6 = 0x8708e;
        uVar3 = 0;
        candidate_1436a();
        candidate_427a();
        candidate_87b2e();
        if ((bool)uVar3) {
          *unaff_A6 = candidate_86f4c;
          uVar3 = 0;
        }
        candidate_87b64();
        if ((bool)uVar3) {
          *unaff_A6 = 0x86b0e;
        }
      }
    }
  }
LAB_0008712e:
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = 0x86eb4;
  }
LAB_0008713e:
  cVar1 = *(char *)((int)unaff_A6 + 0x87);
  *(char *)((int)unaff_A6 + 0x87) = cVar1 + -1;
  if (cVar1 == '\0') {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 0;
  }
  *(short *)(unaff_A6 + 0x20) = *(short *)(unaff_A6 + 0x20) + -1;
  if (*(short *)(unaff_A6 + 0x20) < 0) {
    *(undefined2 *)(unaff_A6 + 0x20) = 0;
  }
  bVar4 = false;
  candidate_4c24();
  if ((bVar4) && ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0)) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_871aa;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x60) {
    *unaff_A6 = candidate_873c0;
  }
  bVar4 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_1945c();
  if (bVar4) {
    *unaff_A6 = candidate_873c0;
  }
  return;
}
